#include "brpch.h"

#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGlBuffer.h"
#include "Brbanje/Core/Core.h"
#include "Brbanje/Core/Log.h"


namespace Brbanje
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BR_CORE_ASSERT(false, "No Renderer api selected")
				
			break;
		case RendererAPI::API::OpenGl:
			return std::make_shared<OpenGlVertexBuffer>(vertices, size);

		}
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BR_CORE_ASSERT(false, "No Renderer api selected")

				break;
		case RendererAPI::API::OpenGl:
			return std::make_shared<OpenGlVertexBuffer>(size);

		}
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BR_CORE_ASSERT(false, "No Renderer api selected")
				break;
		case RendererAPI::API::OpenGl:
			return std::make_shared<OpenGlIndexBuffer>(indices, count);

		}

		return nullptr;

	}


}