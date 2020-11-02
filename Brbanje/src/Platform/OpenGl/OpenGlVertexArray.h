#pragma once

#include <memory>
#include "Brbanje/Renderer/VertexArray.h"



namespace Brbanje
{
	class OpenGlVertexArray : public VertexArray
	{
	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererID;

	public:
		OpenGlVertexArray();
		virtual ~OpenGlVertexArray();
		virtual void Bind() override;
		virtual void Unbind() override;
		
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer) override;

		inline virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers()const override { return m_VertexBuffers;  }
		inline virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer()const override { return m_IndexBuffer; }

	};

}