#pragma once

#include "vkRenderer.h"

// A port of GL infinite plane
struct InfinitePlaneRenderer: public Renderer
{
	InfinitePlaneRenderer(VulkanRenderContext& ctx,
		const std::vector<VulkanTexture>& outputs = std::vector<VulkanTexture> {},
		RenderPass screenRenderPass = RenderPass());

	void fillCommandBuffer(VkCommandBuffer cmdBuffer, size_t currentImage, VkFramebuffer fb = VK_NULL_HANDLE, VkRenderPass rp = VK_NULL_HANDLE) override;
	void updateBuffers(size_t currentImage) override;

	inline void setMatrices(const mat4& proj, const mat4& view, const mat4& model)
  {
    proj_ = proj; view_ = view; model_ = model;
  }

private:
	mat4 proj_;
	mat4 view_;
	mat4 model_;

	struct UniformBuffer
  {
		mat4 proj;
		mat4 view;
		mat4 model;
		float time;
	};
};
