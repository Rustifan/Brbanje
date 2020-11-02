#include "brpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGl/OpenGlRendererAPI.h"

namespace Brbanje
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGlRendererAPI();

}