#include "brpch.h"
#include "Application.h"
#include "GLFW/glfw3.h"
#include "Brbanje/Core/Log.h"
#include "Brbanje/Core/KeyCodes.h"
#include "Input.h"
#include "Brbanje/Renderer/Renderer.h"


namespace Brbanje
{


	Application* Application::s_instance = nullptr;

	

	Application::Application(const std::string& name)
	{
		BR_PROFILE_FUNCTION;
		
		BR_CORE_ASSERT(!s_instance, "Application already exist")
		s_instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create(name));
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
		Renderer::Init();
	}

	Application::~Application()
	{
		for (Layer* layer : m_LayerStack)
		{
			
			delete layer;
		}

	}

	void Application::OnEvent(Event& e)
	{
		BR_PROFILE_FUNCTION;

		EventDispacher dispacher(e);
		dispacher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispacher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPress));
		dispacher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
			{
				break;
			}
			
		}

		
	}

	void Application::Run()
	{
		
		BR_PROFILE_FUNCTION;
		
		
		while (m_Running)
		{

			BR_PROFILE_SCOPE("RUN LOOP");
			float time = (float)glfwGetTime();
			Timestep timestep(time - m_lasTimeFrame);
			m_lasTimeFrame = time;

			
			
			
			

			if (!m_Minimized)
			{
				{
					BR_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)
					{
						layer->OnUpdate(timestep);
						layer->OnRender();
					}
				}
			}


			m_ImGuiLayer->Begin();

			{
				BR_PROFILE_SCOPE("ImGui Layer OnUpdate");
				for (Layer* layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
			}

			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnKeyPress(KeyPressedEvent& e)
	{
		
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		BR_PROFILE_FUNCTION;
		if (e.GetHeight() == 0 || e.GetWidth() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		
		return false;
	}

	void Application::PushLayer(Layer* layer)
	{
		BR_PROFILE_FUNCTION;
		layer->OnAttach();
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		BR_PROFILE_FUNCTION;
		overlay->OnAttach();
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::Close()
	{
		m_Running = false;
	}
}