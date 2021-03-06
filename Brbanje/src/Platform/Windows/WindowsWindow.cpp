#include "brpch.h"
#include "WindowsWindow.h"
#include "Brbanje/Events/ApplicationEvent.h"
#include "Brbanje/Events/MouseEvents.h"
#include "Brbanje/Events/KeyEvent.h"
#include "Brbanje/Core/Log.h"
#include "glad/glad.h"
#include "Platform/OpenGl/OpenGlContext.h"
#include "Brbanje/Renderer/Renderer.h"

namespace Brbanje
{
	
	static bool s_GLFWinitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		BR_CORE_ERROR("GLFW ERROR {0}: {1}", error, description);

	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		BR_PROFILE_FUNCTION;
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		BR_PROFILE_FUNCTION;
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		BR_PROFILE_FUNCTION;
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		BR_CORE_INFO("Creating window - {0} ({1}, {2})", props.Title, props.Width, props.Height);
		if(!s_GLFWinitialized)
		{
			int sucess = glfwInit();
			glfwSetErrorCallback(GLFWErrorCallback);
			BR_CORE_ASSERT(sucess, "Could not initialize GLFW")

			s_GLFWinitialized = true;
		}

#if defined(BR_DEBUG)
		if (Renderer::GetAPI() == RendererAPI::API::OpenGl)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		m_GraphicContext = new OpenGlContext(m_Window);
		m_GraphicContext->Init();
		

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);


		// Event callbacks

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData &data = *(WindowData*) glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;
				
				WindowResizeEvent event(width, height);
				data.EventCallback(event);
				
				
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;

				data.EventCallback(event);
			
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
			{
			
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						break;
					}
					
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.EventCallback(event);
						break;
					}
					
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
				}
			
			
			
			});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods)
			{
			
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			
			
			
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) 
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xoffset, (float)yoffset);
				data.EventCallback(event);
			
			
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) 
			
			{
				
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xpos, (float)ypos);

				data.EventCallback(event);
			
			
			
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character) {
			
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(character);

			data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		BR_PROFILE_FUNCTION;
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		BR_PROFILE_FUNCTION;
		glfwPollEvents();
		m_GraphicContext->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		BR_PROFILE_FUNCTION;
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		m_Data.VSync = enabled;

	}

	bool WindowsWindow::IsVSync()const
	{
		return m_Data.VSync;
	}

	
}