#pragma once

#include "entt.hpp"
#include "Brbanje/Core/Timestep.h"
#include "../../Brbeetor/src/Editor/SceneHierarchyPanel.h"
#include "SceneCamera.h"
#include "Components.h"
#include "Gizmo.h"
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
		glm::vec2 m_ViewportWindowPos;
		bool m_MouseHoveredOnVIewport = false;
		TransformComponent* m_MainCameraTransform = nullptr;
		std::stringstream m_saveBuffer;
		std::unordered_map<std::string, Ref<Texture2D>> m_TextureMap;
		SceneCamera* m_MainCamera = nullptr;
		Gizmo m_Gizmo;
		
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
		void SetViewportWindowPos(const glm::vec2& windowPos) { m_ViewportWindowPos = windowPos; }
		bool IsClicked(TransformComponent& transform, const glm::vec2& mousePos);
		Ref<Texture2D> GetTextureFromTextureMap(const std::string& filePath);
		glm::vec2 GetSceneMousePos();
		SceneCamera* GetMainCamera()const { return m_MainCamera; }
		void SetMouseHoveredOnVieport(bool isHovered) { m_MouseHoveredOnVIewport = isHovered; }
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class Serializer;
	};
}