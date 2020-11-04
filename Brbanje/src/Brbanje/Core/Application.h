#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Brbanje/Events/Event.h"
#include "Brbanje/Events/ApplicationEvent.h"
#include "Brbanje/Events/KeyEvent.h"
#include "imgui/ImGuiLayer.h"
#include "Brbanje/Core/Timestep.h"



namespace Brbanje
{
	
	class Brbanje_API Application
	{
	private:
		static Application* s_instance;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		float m_lasTimeFrame = 0.0f;
	private:
		bool OnWindowClose(WindowCloseEvent &e);
		bool OnKeyPress(KeyPressedEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		
		
	public:
		Application(const std::string& name = "Brbanje Engine");
		virtual ~Application();
		void Close();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		static inline Application& getApplication() { return *s_instance; };
		inline Window& getWindow() { return  *m_Window; }
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		void Run();
		
		
		
		
	};
	
	//To be defined in client
	Application* CreateApplication();

}
