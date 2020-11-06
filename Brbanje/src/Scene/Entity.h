#pragma once

#include "entt.hpp"
#include "Scene/Scene.h"

namespace Brbanje
{
	class Entity
	{
	private:
		
		Scene* m_Scene = nullptr;
		entt::entity m_EntityHandle{entt::null};
	public:
		Entity(entt::entity entityHandle, Scene* scene);	
		Entity() = default;

		template<typename T>
		T& GetComponent()
		{
			BR_CORE_ASSERT(HasComponent<T>(), "Entity does not have component ")
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}
		
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			BR_CORE_ASSERT(!HasComponent<T>(), "it already have that component ")

			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template <typename T>
		void RemoveComponent()
		{
			BR_CORE_ASSERT(HasComponent<T>(), "Entity does not have component ")

			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_Scene != nullptr; }
	};

}