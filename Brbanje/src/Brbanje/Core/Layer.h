#pragma once

#include "Brbanje/Core/Core.h"
#include "Brbanje/Events/Event.h"
#include "Brbanje/Core/Timestep.h"

namespace Brbanje
{
	class Brbanje_API Layer
	{
	private:
		std::string m_DebugName;
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		virtual void OnAttach() {}
		virtual void OnDetach(){}
		virtual void OnUpdate(Timestep ts){}
		virtual void OnImGuiRender(){}
		virtual void OnEvent(Event& event){}
		inline const std::string& GetName()const { return m_DebugName; }



	};




}