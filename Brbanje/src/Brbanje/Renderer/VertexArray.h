#pragma once

#include <memory>
#include "Brbanje/Renderer/Buffer.h"

namespace Brbanje
{
	class VertexArray
	{
	
	public:

		virtual ~VertexArray() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		static Ref<VertexArray> Create();
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void AddIndexBuffer(const Ref<IndexBuffer> indexBuffer) = 0;
		//getters
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer()const = 0;

	};

}