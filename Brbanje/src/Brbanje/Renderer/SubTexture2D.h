#pragma once
#include "glm/glm.hpp"
#include "Texture.h"

namespace Brbanje
{
	class SubTexture2D
	{
	public:
		SubTexture2D(Ref<Texture2D> texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D> GetTexture()const { return m_Texture; }
		const glm::vec2* GetTetureCoords()const { return m_TextureCoords; }

		static Ref<SubTexture2D> CreateFromCoords(Ref<Texture2D> texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1.0f,1.0f });
	private:

		Ref<Texture2D> m_Texture;
		glm::vec2 m_TextureCoords[4];

	};

}