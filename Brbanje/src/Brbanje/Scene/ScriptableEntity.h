#pragma once

#include "Brbanje/Scene/Entity.h"
#include "Brbanje/Core/Timestep.h"

namespace Brbanje
{
	

	class ScriptableEntity
	{

	private:
		Entity m_Entity;
		
	public:
		virtual void OnCreate() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnDestroy() {}
		
		template <class T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		friend class Scene;
	};
}