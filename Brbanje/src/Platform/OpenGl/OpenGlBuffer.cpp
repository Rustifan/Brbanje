#pragma once

#include "brpch.h"
#include "OpenGlBuffer.h"


namespace Brbanje
{
	// Vertex Buffer

	OpenGlVertexBuffer::OpenGlVertexBuffer(float* vertices, uint32_t size)
	{
		BR_PROFILE_FUNCTION;

		glGenBuffers(1, &RendererID);
		glBindBuffer(GL_ARRAY_BUFFER,RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	}

	OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size)
	{
		BR_PROFILE_FUNCTION;

		glGenBuffers(1, &RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	}

	OpenGlVertexBuffer::~OpenGlVertexBuffer()
	{
		BR_PROFILE_FUNCTION;

		glDeleteBuffers(1, &RendererID);
	}

	void OpenGlVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	}

	void OpenGlVertexBuffer::Bind()
	{
		BR_PROFILE_FUNCTION;
		glBindBuffer(GL_ARRAY_BUFFER, RendererID);
	}

	void OpenGlVertexBuffer::Unbind()
	{
		BR_PROFILE_FUNCTION;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGlVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
		
		
	}

	const BufferLayout& OpenGlVertexBuffer::GetLayout() const
	{
		return m_Layout;
	}



	//Index buffer

	OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t* indices, uint32_t count): m_count(count)
	{
		glGenBuffers(1, &RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*count, indices, GL_STATIC_DRAW);
	}

	OpenGlIndexBuffer::~OpenGlIndexBuffer()
	{
		glDeleteBuffers(1, &RendererID);
	}

	void OpenGlIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID);
	}

	void OpenGlIndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	
}