#pragma once
#include <string>
#include "Brbanje/Renderer/Texture.h"
#include "glad/glad.h"

namespace Brbanje
{
	class OpenGlTexture2D : public Texture2D
	{
	private:
		std::string m_FilePath;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	public:
		OpenGlTexture2D(const std::string& fileName);
		OpenGlTexture2D(uint32_t width, uint32_t height);
		
		virtual ~OpenGlTexture2D();
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight()const override { return m_Height; }
		virtual uint32_t GetRendererID()const override { return m_RendererID; }
		virtual void Bind(uint32_t slot = 0) const override;
		virtual void  SetData(void* data, uint32_t size) const override;
		virtual const std::string& GetFilePath()const override { return m_FilePath; }
		virtual bool operator==(const Texture& other)override { return m_RendererID == other.GetRendererID();  }

	};	



}