#include "brpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGlVertexArray.h"

namespace Brbanje
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BR_CORE_ASSERT(false, "No Renderer api selected")

				break;
		case RendererAPI::API::OpenGl:
			return std::make_shared <OpenGlVertexArray>();

		}
		return nullptr;

	}

}