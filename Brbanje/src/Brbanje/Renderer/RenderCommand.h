#pragma once

#include "RendererAPI.h"
#include "Renderer2D.h"

namespace Brbanje
{
	class RenderCommand
	{
	private:
		static RendererAPI* s_RendererAPI;
	public:
		inline static void Clear() { s_RendererAPI->Clear(); }
		inline static void SetColor(const glm::vec4& color) { s_RendererAPI->SetColor(color); }
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) { s_RendererAPI->DrawIndexed(vertexArray, indexCount); }
		inline static void Init() { s_RendererAPI->Init(); Renderer2D::Init(); }
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RendererAPI->SetViewport(x, y, width, height); }
	};
}