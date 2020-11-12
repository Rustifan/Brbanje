#pragma once
#include "Brbanje/Core/Core.h"
#include "Brbanje/Core/Layer.h"

#include "Brbanje/Events/Event.h"
#include "Brbanje/Events/MouseEvents.h"
#include "Brbanje/Events/KeyEvent.h"
#include "Brbanje/Events/ApplicationEvent.h"

namespace Brbanje
{ 
	class Brbanje_API ImGuiLayer: public Layer
	{
	private:
		float m_Time = 0;
		bool m_BlockEvents = true;
	private:
		
	public:
		ImGuiLayer();
		~ImGuiLayer();
		
		virtual void OnAttach()override;
		virtual void OnDetach()override;
		virtual void OnImGuiRender()override;
		virtual void OnEvent(Event& e)override;
		void SetDarkThemeColor();
		void Begin();
		void End();
		void SetBlockEvents(bool block) { m_BlockEvents = block; }
	};
}
