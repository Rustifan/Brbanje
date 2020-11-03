#include "brpch.h"
#include "Framebuffer.h"
#include "Platform/OpenGl/OpenGlFramebuffer.h"
#include "Renderer.h"

namespace Brbanje
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecifications& frameBufferSpecs)
	{
		
		
			switch (Renderer::GetAPI())
			{
			case RendererAPI::API::None:
				BR_CORE_ASSERT(false, "No Renderer api selected")

					break;
			case RendererAPI::API::OpenGl:
				return std::make_shared <OpenGlFramebuffer>(frameBufferSpecs);

			}
			return nullptr;

		


	}
}