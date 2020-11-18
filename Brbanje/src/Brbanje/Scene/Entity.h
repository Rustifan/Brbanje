#pragma once




#include "Brbanje/Core/Core.h"
#include "Brbanje/Core/Log.h"
#include "entt.hpp"



namespace Brbanje
{	
	
	class Scene;

	class Entity
	{
	private:
		
		Scene* m_Scene = nullptr;
		entt::entity m_EntityHandle{entt::null};
	public:
		Entity(entt::entity entityHandle, Scene* scene);	
		Entity() = default;

		Scene* GetScene() { return m_Scene; }
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
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);

			m_Scene->OnComponentAdded<T>(component, *this);

			return component;
		}

		template <typename T>
		void RemoveComponent()
		{
			BR_CORE_ASSERT(HasComponent<T>(), "Entity does not have component ")

			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_Scene != nullptr; }
		operator uint32_t() const{ return (uint32_t)m_EntityHandle; }
		operator entt::entity() const{ return m_EntityHandle; }
		bool operator==(const Entity& other) const 
		{ 
			if (!m_Scene || !(other.m_Scene))
			{
				return false;
			}
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; 
		}
		bool operator!=(const Entity& other) const { return !(*this == other); }
	};

}