#include "brpch.h"
#include "OpenGlContext.h"
#include "GLFW/glfw3.h"
#include "GLAD/glad.h"
#include "Brbanje/Core/Core.h"
#include "Brbanje/Core/Log.h"

namespace Brbanje
{
	OpenGlContext::OpenGlContext(GLFWwindow* windowHandle): m_WindowHandle(windowHandle)
	{
		BR_CORE_ASSERT(m_WindowHandle, "Window Handle is NULL")
	}

	void OpenGlContext::Init()
	{
		BR_PROFILE_FUNCTION;
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		BR_CORE_ASSERT(status, "Failed to init GLAD")

		BR_CORE_INFO("OpenGl Vendor: {0}", glGetString(GL_VENDOR));
		BR_CORE_INFO("OpenGl Renderer: {0}", glGetString(GL_RENDERER));
		BR_CORE_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGlContext::SwapBuffers()
	{
		BR_PROFILE_FUNCTION;
		glfwSwapBuffers(m_WindowHandle);
	}
}