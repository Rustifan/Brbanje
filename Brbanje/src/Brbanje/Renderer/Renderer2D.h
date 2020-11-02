#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace Brbanje
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		//primitives
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color );
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color );
		
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<Texture2D> texture, float tiling = 1.0f, glm::vec4 tintingColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<Texture2D		> texture, float tiling=1.0f, glm::vec4 tintingColor = glm::vec4(1.0f));

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D> texture, float tiling = 1.0f, glm::vec4 tintingColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture, float tiling = 1.0f, glm::vec4 tintingColor = glm::vec4(1.0f));

		struct Statistics
		{
			uint32_t DrawCalls;
			uint32_t QuadNumber;

			uint32_t VertexCount() { return QuadNumber * 4; }
			uint32_t IndexCount() { return QuadNumber * 6; }
		};

		static const Statistics &GetStats();
		static void ResetStats();

	private:
		static void FlushAndReset();
	};
}