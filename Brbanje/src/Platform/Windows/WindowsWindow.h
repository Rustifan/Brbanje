#pragma once
#include "Brbanje/Core/Window.h"
#include "Brbanje/Renderer/GraphicContest.h"
#include "GLFW/glfw3.h"

namespace Brbanje
{
	class  WindowsWindow : public Window
	{
	private:
		GLFWwindow* m_Window;
		GraphicContext* m_GraphicContext;
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		}m_Data;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();
		void OnUpdate() override;

		inline virtual void* GetNativeWindow()const override { return m_Window; }
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		
		//window atrib
		inline void SetEventCallback(const EventCallbackFn& callback)override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;
	};

}
