#include "brpch.h"
#include "OpenGlVertexArray.h"
#include "Brbanje/Renderer/Buffer.h"
#include "Glad/glad.h"



namespace Brbanje
{

	static inline GLenum ShaderDataTypeToGlEnum(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:				return GL_FLOAT;
		case ShaderDataType::Float2:			return GL_FLOAT;
		case ShaderDataType::Float3:			return GL_FLOAT;
		case ShaderDataType::Float4:			return GL_FLOAT;
		case ShaderDataType::Mat3:				return GL_FLOAT;
		case ShaderDataType::Mat4:				return GL_FLOAT;
		case ShaderDataType::Int:				return GL_INT;
		case ShaderDataType::Int2:				return GL_INT;
		case ShaderDataType::Int3:				return GL_INT;
		case ShaderDataType::Int4:				return GL_INT;
		case ShaderDataType::Bool:				return GL_BOOL;
		}

		BR_CORE_ASSERT(false, "Unown data type")
			return 0;
	}

	Brbanje::OpenGlVertexArray::OpenGlVertexArray()
	{
		BR_PROFILE_FUNCTION;
		glGenVertexArrays(1, &m_RendererID);

	}

	Brbanje::OpenGlVertexArray::~OpenGlVertexArray()
	{
		BR_PROFILE_FUNCTION;
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void Brbanje::OpenGlVertexArray::Bind()
	{
		BR_PROFILE_FUNCTION;
		glBindVertexArray(m_RendererID);
	}

	void Brbanje::OpenGlVertexArray::Unbind()
	{
		BR_PROFILE_FUNCTION;
		glBindVertexArray(0);
	}

	void Brbanje::OpenGlVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		BR_PROFILE_FUNCTION;
		BR_CORE_ASSERT(vertexBuffer->GetLayout().GetBufferEllements().size(), "vertex buffer must have layout set");
		vertexBuffer->Bind();

		int i = 0;
		for (BufferElement element : vertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i,
				element.GetComponentCount(),
				ShaderDataTypeToGlEnum(element.Type),
				element.normalized ? GL_TRUE : GL_FALSE,
				vertexBuffer->GetLayout().GetStride(),
				(const void*)element.Offset);

			++i;

		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void Brbanje::OpenGlVertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer)
	{
		BR_PROFILE_FUNCTION;
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}