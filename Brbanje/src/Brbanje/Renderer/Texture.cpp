#include "brpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGlTexture.h"

namespace Brbanje
{
	Ref<Texture2D> Texture2D::Create(const std::string& fileName)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BR_CORE_ASSERT(false, "No Renderer api selected")

				break;
		case RendererAPI::API::OpenGl:
			return std::make_shared<OpenGlTexture2D>(fileName);

		}
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BR_CORE_ASSERT(false, "No Renderer api selected")

				break;
		case RendererAPI::API::OpenGl:
			return std::make_shared<OpenGlTexture2D>(width, height);

		}
		return nullptr;
	}
}