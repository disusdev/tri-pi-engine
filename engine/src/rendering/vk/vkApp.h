#pragma once

#define VK_NO_PROTOTYPES
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <deque>
#include <functional>
#include <memory>
#include <limits>

#include <imgui.h>

// #include "shared/Camera.h"
#include "../api/utils.h"
#include "../../math/mathm_wrapper.hpp"

#include "vkUtils.h"

#include "vkResources.h"

// #include <glm/glm.hpp>
// #include <glm/ext.hpp>
// using glm::mat4;
// using glm::vec2;
// using glm::vec3;
// using glm::vec4;

#include "../camera.h"

GLFWwindow* initVulkanApp(int width, int height, Resolution* resolution = nullptr);

bool drawFrame(VulkanRenderDevice& vkDev, const std::function<void(uint32_t)>& updateBuffersFunc, const std::function<void(VkCommandBuffer, uint32_t)>& composeFrameFunc);

struct Renderer;

struct RenderItem {
	Renderer& renderer_;
	bool enabled_ = true;
	bool useDepth_ = true;
	explicit RenderItem(Renderer& r, bool useDepth = true)
	: renderer_(r)
	, useDepth_(useDepth)
	{}
};

struct VulkanRenderContext
{
	VulkanInstance vk;
	VulkanRenderDevice vkDev;
	VulkanContextCreator ctxCreator;
	VulkanResources resources;

	VulkanRenderContext(void* window, uint32_t screenWidth, uint32_t screenHeight, const VulkanContextFeatures& ctxFeatures = VulkanContextFeatures()):
		ctxCreator(vk, vkDev, window, screenWidth, screenHeight, ctxFeatures),
		resources(vkDev),

		depthTexture(resources.addDepthTexture(vkDev.framebufferWidth, vkDev.framebufferHeight, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)),

		screenRenderPass(resources.addFullScreenPass()),
		screenRenderPass_NoDepth(resources.addFullScreenPass(false)),

		finalRenderPass(resources.addFullScreenPass(true, RenderPassCreateInfo { .clearColor_ = false, .clearDepth_ = false, .flags_ = eRenderPassBit_Last  })),
		clearRenderPass(resources.addFullScreenPass(true, RenderPassCreateInfo { .clearColor_ =  true, .clearDepth_ =  true, .flags_ = eRenderPassBit_First })),

		swapchainFramebuffers(resources.addFramebuffers(screenRenderPass.handle, depthTexture.image.imageView)),
		swapchainFramebuffers_NoDepth(resources.addFramebuffers(screenRenderPass_NoDepth.handle))
	{
	}

	void updateBuffers(uint32_t imageIndex);
	void composeFrame(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	// For Chapter 8 & 9
	inline PipelineInfo pipelineParametersForOutputs(const std::vector<VulkanTexture>& outputs) const {
		return PipelineInfo {
			.width = outputs.empty() ? vkDev.framebufferWidth : outputs[0].width,
			.height = outputs.empty() ? vkDev.framebufferHeight : outputs[0].height,
			.useBlending = false
		};
	}

	std::vector<RenderItem> onScreenRenderers_;

	VulkanTexture depthTexture;

	// Framebuffers and renderpass for on-screen rendering
	RenderPass screenRenderPass;
	RenderPass screenRenderPass_NoDepth;

	RenderPass clearRenderPass;
	RenderPass finalRenderPass;

	std::vector<VkFramebuffer> swapchainFramebuffers;
	std::vector<VkFramebuffer> swapchainFramebuffers_NoDepth;

	void beginRenderPass(VkCommandBuffer cmdBuffer, VkRenderPass pass, size_t currentImage, const VkRect2D area,
		VkFramebuffer fb = VK_NULL_HANDLE,
		uint32_t clearValueCount = 0, const VkClearValue* clearValues = nullptr)
	{
		const VkRenderPassBeginInfo renderPassInfo = {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.renderPass = pass,
			.framebuffer = (fb != VK_NULL_HANDLE) ? fb : swapchainFramebuffers[currentImage],
			.renderArea = area,
			.clearValueCount = clearValueCount,
			.pClearValues = clearValues
		};

		vkCmdBeginRenderPass( cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );
	}
};

struct VulkanApp
{
	VulkanApp(int screenWidth, int screenHeight, const VulkanContextFeatures& ctxFeatures = VulkanContextFeatures())
		: window_(initVulkanApp(screenWidth, screenHeight, &resolution_)),
		ctx_(window_, resolution_.width, resolution_.height, ctxFeatures),
		onScreenRenderers_(ctx_.onScreenRenderers_)
	{
		glfwSetWindowUserPointer(window_, this);
		assignCallbacks();
	}

	~VulkanApp()
	{
		glslang_finalize_process();
		glfwTerminate();
	}

	virtual void drawUI() {}
	virtual void draw3D() = 0;

	void mainLoop();

	// Check if none of the ImGui widgets were touched so our app can process mouse events
	inline bool shouldHandleMouse() const { return !ImGui::GetIO().WantCaptureMouse; }

	void
	set_mouse_pos(f32 x, f32 y);

	void
	set_mouse_pos_relative(f32 x, f32 y);

	void
	gltf_key_callback(int key, int scancode, int action, int mods);

	void
	gltf_set_mouse_button_callback(i32 button, i32 action, i32 mods);

	virtual void update(float deltaSeconds) = 0;

	inline float getFPS() const { return 60.0f/* fpsCounter_.getFPS()*/; }

protected:
	t_button_state button_state = { 0 };
	vec2 relative_point = vec2_zero();

	Resolution resolution_;
	GLFWwindow* window_ = nullptr;
	VulkanRenderContext ctx_;
	std::vector<RenderItem>& onScreenRenderers_;

private:
	void assignCallbacks();

	void updateBuffers(uint32_t imageIndex);
};

struct CameraApp: public VulkanApp
{
	CameraApp(int screenWidth, int screenHeight, const VulkanContextFeatures& ctxFeatures = VulkanContextFeatures()) :
		VulkanApp(screenWidth, screenHeight, ctxFeatures)
	{}

	virtual void update(float deltaSeconds) override
	{
		if (!shouldHandleMouse()) return;

		f64 xpos, ypos;
		glfwGetCursorPos(window_, &xpos, &ypos);

		input.current_mouse_position.x = xpos;
		input.current_mouse_position.y = ypos;

		input_update(&input, *(i32*)&button_state);

		if (input_get_key_down(&input, KEY_CODE_MRB))
		{
			relative_point = input.current_mouse_position;
			input.center_position = input.current_mouse_position;
			glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else
		if (input_get_key(&input, KEY_CODE_MRB))
		{
			input.rotation_delta = vec2_sub(input.current_mouse_position, input.center_position);
			input.rotation_delta = vec2_mul_scalar(input.rotation_delta, 10.0f);
			set_mouse_pos_relative(0.0f, 0.0f);
		}
		else
		if (input_get_key_up(&input, KEY_CODE_MRB))
		{
			input.rotation_delta = vec2_zero();
			input.center_position = vec2_zero();
			set_mouse_pos_relative(0.0f, 0.0f);
			glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (input_get_key_down(&input, KEY_CODE_SPACE))
		{
			// hide next mesh
			// g_fps_graph.visible = !g_fps_graph.visible;
			// tex_state = ++tex_state % TEXTURE_STATE_COUNT;
		}

		camera_update(&camera, &input, (f64)deltaSeconds);
	}

	mat4 getDefaultProjection() const
	{
		const float ratio = ctx_.vkDev.framebufferWidth / (float)ctx_.vkDev.framebufferHeight;
		return camera_get_projection_matrix(ratio, 75.0f, false, true);
	}

protected:
	t_camera camera = { 0 };
	t_input_state input = { 0 };
};
