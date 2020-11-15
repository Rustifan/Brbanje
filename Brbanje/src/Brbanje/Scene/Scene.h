#pragma once

#include "entt.hpp"
#include "Brbanje/Core/Timestep.h"
#include "../../Brbeetor/src/Editor/SceneHierarchyPanel.h"
#include <sstream>

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
		
		std::stringstream m_saveBuffer;
		
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
		const std::stringstream& GetSaveBuffer()const { return m_saveBuffer; }
		void ClearSaveBuffer() { m_saveBuffer.str(std::string()); }
		
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class Serializer;
	};
}