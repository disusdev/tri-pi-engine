#include <defines.h>

#include "entry.h"

#include <rendering/renderers/GuiRenderer.h>
#include <rendering/renderers/MultiRenderer.h>
#include <rendering/renderers/InfinitePlaneRenderer.h>

#include <ImGuizmo.h>

class
Editor : public CameraApp
{
public:
  Editor() : CameraApp(1280, 800)// CameraApp(-95, -95)
		, envMap(ctx_.resources.loadCubeMap("data/piazza_bologni_1k.hdr"))
		, irrMap(ctx_.resources.loadCubeMap("data/piazza_bologni_1k_irradiance.hdr"))
		, sceneData(ctx_, "data/meshes/test_graph.meshes", "data/meshes/test_graph.scene", "data/meshes/test_graph.materials", envMap, irrMap)
		, testSceneData(ctx_, "data/meshes/test.meshes", "data/meshes/test.scene", "data/meshes/test.materials", envMap, irrMap)
		, test2SceneData(ctx_, "data/meshes/test2.meshes", "data/meshes/test2.scene", "data/meshes/test2.materials", envMap, irrMap)
		, plane(ctx_)
		, multiRenderer(ctx_, sceneData)
		, testMultiRenderer(ctx_, testSceneData)
		, test2MultiRenderer(ctx_, test2SceneData)
		, imgui(ctx_)
  {
		camera.position = vec3_create(0.0f, 5.0f, 10.0f);

		onScreenRenderers_.emplace_back(plane, false);
		onScreenRenderers_.emplace_back(multiRenderer);
		onScreenRenderers_.emplace_back(testMultiRenderer);
		onScreenRenderers_.emplace_back(test2MultiRenderer);
		onScreenRenderers_.emplace_back(imgui, false);

		sceneData.scene_.localTransform_[0] = quat_to_mat4( quat_from_euler(vec3_create((float)(PI / 2.f), 0.f, 0.0f)) ); // glm::rotate(glm::mat4(1.f), (float)(M_PI / 2.f), glm::vec3(1.f, 0.f, 0.0f));
  }

	void drawUI() override
	{
		ImGui::Begin("Information", nullptr);
			ImGui::Text("FPS: %.2f", getFPS());
		ImGui::End();

		ImGui::Begin("Scene graph", nullptr);
		int node = renderSceneTree(sceneData.scene_, 0);
		if (node > -1)
			selectedNode = node;
		ImGui::End();

		editNode(selectedNode);
	}

	void draw3D() override
	{
		const mat4 p = getDefaultProjection();
		const mat4 view = camera_get_view_matrix(&camera);

		multiRenderer.setMatrices(p, view);
		multiRenderer.setCameraPosition(camera.position);
		testMultiRenderer.setMatrices(p, view);
		testMultiRenderer.setCameraPosition(camera.position);
		test2MultiRenderer.setMatrices(p, view);
		test2MultiRenderer.setCameraPosition(camera.position);
		plane.setMatrices(p, view, mat4_identity());
	}

	void update(float deltaSeconds) override
	{
		CameraApp::update(deltaSeconds);

		// update/upload matrices for individual scene nodes
		sceneData.recalculateAllTransforms();
		sceneData.uploadGlobalTransforms();
	}

private:

	int selectedNode = -1;

	VulkanTexture envMap;
	VulkanTexture irrMap;

	VKSceneData sceneData;
	VKSceneData testSceneData;
	VKSceneData test2SceneData;

	InfinitePlaneRenderer plane;
	MultiRenderer multiRenderer;
	MultiRenderer testMultiRenderer;
	MultiRenderer test2MultiRenderer;

	GuiRenderer imgui;

	void editNode(int node)
	{
		if (node < 0)
			return;

		const float ratio = ctx_.vkDev.framebufferWidth / (float)ctx_.vkDev.framebufferHeight;
		mat4 cameraProjection = camera_get_projection_matrix(ratio, 75.0f, false, false);// getDefaultProjection();
		// mat4 cameraView = camera.getViewMatrix();
		const mat4 cameraView = camera_get_view_matrix(&camera);

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::BeginFrame();

		std::string name = getNodeName(sceneData.scene_, node);
		std::string label = name.empty() ? (std::string("Node") + std::to_string(node)) : name;
		label = "Node: " + label;

		ImGui::Begin("Editor");
		ImGui::Text("%s", label.c_str());

		mat4 globalTransform = sceneData.scene_.globalTransform_[node]; // fetch global transform
		mat4 srcTransform = globalTransform;
		mat4 localTransform = sceneData.scene_.localTransform_[node];

		ImGui::Separator();
		ImGuizmo::SetID(1);

		editTransform(cameraView, cameraProjection, globalTransform);
		mat4 deltaTransform = mat4_inverse(srcTransform) * globalTransform;  // calculate delta for edited global transform
		sceneData.scene_.localTransform_[node] = localTransform * deltaTransform; // modify local transform
		markAsChanged(sceneData.scene_, node);

		ImGui::Separator();
		ImGui::Text("%s", "Material");

		editMaterial(node);
		ImGui::End();
	}

	void editTransform(const mat4& view, const mat4& projection, mat4& matrix)
	{
		static ImGuizmo::OPERATION gizmoOperation(ImGuizmo::TRANSLATE);

		if (ImGui::RadioButton("Translate", gizmoOperation == ImGuizmo::TRANSLATE))
			gizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", gizmoOperation == ImGuizmo::ROTATE))
			gizmoOperation = ImGuizmo::ROTATE;

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

		// ImGuizmo::DrawGrid(view.data, projection.data, mat4_identity().data, 100.f);
		
		// Draw bbox of an object?
		// ImGuizmo::DrawCubes(view.data, projection.data, matrix.data, 1);

		ImGuizmo::Manipulate(view.data, projection.data, gizmoOperation, ImGuizmo::WORLD, matrix.data);
	}

	void editMaterial(int node)
	{
		if (!sceneData.scene_.materialForNode_.contains(node))
			return;

		auto matIdx = sceneData.scene_.materialForNode_.at(node);
		MaterialDescription& material = sceneData.materials_[matIdx];

		float emissiveColor[4];
		memcpy(emissiveColor, &material.emissiveColor_, sizeof(vec4));
		vec4 oldColor = material.emissiveColor_;
		ImGui::ColorEdit3("Emissive color", emissiveColor);

		if (memcmp(emissiveColor, &oldColor, sizeof(vec4)))
		{
			memcpy(&material.emissiveColor_, emissiveColor, sizeof(vec4));
			sceneData.updateMaterial(matIdx);
		}
	}
};

CameraApp* CreateApp(AppCMDArgs args)
{
  return new Editor();
}