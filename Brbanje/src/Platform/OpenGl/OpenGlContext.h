#pragma once

#include "Brbanje/Renderer/GraphicContest.h"

struct GLFWwindow;

namespace Brbanje
{

	class OpenGlContext : public GraphicContext
	{
	private:
		GLFWwindow* m_WindowHandle;
	public:
		OpenGlContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	};

}