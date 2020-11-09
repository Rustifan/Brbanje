#pragma once

#include "Brbanje/Scene/Entity.h"

namespace Brbanje
{
	class ScriptableEntity
	{
	private:
		Entity m_Entity;
		
	public:
		
		
		template <class T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		friend class Scene;
	};
}