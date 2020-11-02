#include "brpch.h"
#include "OpenGlTexture.h"
#include "glad/glad.h"


#define STB_IMAGE_IMPLEMENTATION
#include "../Vendor/stbImage/stb_image.h"


namespace Brbanje
{
	OpenGlTexture2D::OpenGlTexture2D(const std::string& filePath): m_FilePath(filePath), m_DataFormat(GL_RGBA), m_InternalFormat(GL_RGBA8)
	{
		BR_PROFILE_FUNCTION;
		
		int width, height, chanells;

		stbi_uc* data = nullptr;
		stbi_set_flip_vertically_on_load(true);
		{
			BR_PROFILE_SCOPE("stbi_load  Brbanje::OpenGlTexture2D(const std::string&)");
			data = stbi_load(filePath.c_str(), &width, &height, &chanells, 0);
		}
		

		if (chanells == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		else if (chanells == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}

		BR_CORE_ASSERT(m_InternalFormat, "unknown internal image format")
		BR_CORE_ASSERT(m_DataFormat, "unknown data image format")

		BR_CORE_ASSERT(data, "stbi could not load image at {0}", filePath)

		m_Width = width;
		m_Height = height;
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
		
			
		stbi_image_free(data);
	}

	OpenGlTexture2D::OpenGlTexture2D(uint32_t width, uint32_t height): m_Width(width), m_Height(height)
	{
		BR_PROFILE_FUNCTION;

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
		
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		
	}

	OpenGlTexture2D::~OpenGlTexture2D()
	{
		BR_PROFILE_FUNCTION;
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGlTexture2D::Bind(uint32_t slot)const
	{
		BR_PROFILE_FUNCTION;
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGlTexture2D::SetData(void* data, uint32_t size)const
	{
		BR_PROFILE_FUNCTION;

		int bytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
		BR_CORE_ASSERT(size == m_Width * m_Height * bytesPerPixel, "size must be a size of Texture")

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
}