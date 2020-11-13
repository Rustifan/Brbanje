#pragma once

#include "entt.hpp"
#include "Brbanje/Core/Timestep.h"
#include "../../Brbeetor/src/Editor/SceneHierarchyPanel.h"


namespace Brbanje
{	
	class SceneHierarchyPanel;
		  

	class Entity;
	
	class Scene
	{
	private:
		entt::registry m_Registry;
		SceneHierarchyPanel* m_Panel = nullptr;
		
		uint32_t m_ViewportWidth, m_ViewPortHeight;
	public:
		Scene();
		~Scene();
		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);
		Entity CreateEntity(const std::string& tag = std::string());
		void DestroyEntity(Entity entity);
		void NewScene();
		template <typename T>
		void OnComponentAdded(T& component, Entity entity);

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class Serializer;
	};
}