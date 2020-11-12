#pragma once

#include "brpch.h"

#include "Brbanje/Core/Core.h"
#include "Brbanje/Events/Event.h"

namespace Brbanje
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Brbanje engine", unsigned int width = 1600, unsigned int height = 900 ):
			Title(title), Width(width), Height(height){}
	};

	
	class Brbanje_API Window
	{
		
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() = default;
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual void* GetNativeWindow() const = 0;

		//Window Atrib
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		static Window* Create(const WindowProps& props = WindowProps());
		

	};

}