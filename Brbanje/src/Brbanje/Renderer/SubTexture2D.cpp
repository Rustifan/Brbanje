#include "brpch.h"
#include "SubTexture2D.h"

namespace Brbanje
{
	SubTexture2D::SubTexture2D(Ref<Texture2D> texture, const glm::vec2& min, const glm::vec2& max) :
		m_Texture(texture)
	{
		m_TextureCoords[0] = { min.x, min.y };
		m_TextureCoords[1] = { max.x, min.y };
		m_TextureCoords[2] = { max.x, max.y };
		m_TextureCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(Ref<Texture2D> texture, const glm::vec2& coords,const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		glm::vec2 min = { (coords.x * cellSize.x)/ texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((coords.x + spriteSize.x)*cellSize.x) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y ) / texture->GetHeight() };

		return std::make_shared<SubTexture2D>(texture, min, max);
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromPixCoords(Ref<Texture2D> texture, const glm::vec2& minPix, const glm::vec2& maxPix)
	{
		glm::vec2 min = glm::vec2(minPix.x / texture->GetWidth(), minPix.y / texture->GetHeight());
		glm::vec2 max = glm::vec2(maxPix.x / texture->GetWidth(), maxPix.y / texture->GetHeight());
		return std::make_shared<SubTexture2D>(texture, min, max);
	}

	void SubTexture2D::SetTextureCoords(const glm::vec2& min, const glm::vec2& max)
	{
		m_TextureCoords[0] = { min.x, min.y };
		m_TextureCoords[1] = { max.x, min.y };
		m_TextureCoords[2] = { max.x, max.y };
		m_TextureCoords[3] = { min.x, max.y };

	}
}