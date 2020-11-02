#pragma once

#include "Brbanje/Renderer/Buffer.h"
#include "glad/glad.h"

namespace Brbanje
{
	class OpenGlVertexBuffer : public VertexBuffer
	{
	public:
		OpenGlVertexBuffer(float* vertices, uint32_t size);
		OpenGlVertexBuffer(uint32_t size);
		virtual ~OpenGlVertexBuffer();
		virtual void SetData(const void* data, uint32_t size)override;
		virtual void Bind()override;
		virtual void Unbind()override;
		virtual void SetLayout(const BufferLayout& layout)override;
		virtual const BufferLayout& GetLayout()const override;
	private:
		uint32_t RendererID;
		BufferLayout m_Layout;

	};

	class OpenGlIndexBuffer : public IndexBuffer
	{
	public:
		OpenGlIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGlIndexBuffer();
		virtual void Bind()override;
		virtual void Unbind()override;
		inline uint32_t GetCount()const { return m_count; }
	private:
		uint32_t RendererID;
		uint32_t m_count;
	};
}