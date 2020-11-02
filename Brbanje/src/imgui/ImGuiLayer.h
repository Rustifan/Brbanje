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
	private:
		
	public:
		ImGuiLayer();
		~ImGuiLayer();
		
		virtual void OnAttach()override;
		virtual void OnDetach()override;
		virtual void OnImGuiRender()override;
		
		void Begin();
		void End();
	};
}
