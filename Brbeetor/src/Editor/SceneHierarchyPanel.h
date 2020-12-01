#pragma once

#include "Brbanje/Core/Core.h"
#include "Brbanje/Scene/Entity.h"
#include "glm/glm.hpp"

namespace Brbanje
{
	class Scene;

	class SceneHierarchyPanel
	{
	private:
		Ref<Scene> m_Scene;
		Entity m_EntitySelectionContext = Entity();

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	public:

		SceneHierarchyPanel(Ref<Scene> scene);
		SceneHierarchyPanel() = default;
		void SetContext(Ref<Scene> constext);
		void OnImGuiRender();
		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 1.0f, float columnWidth = 100.0f);
		friend class Scene;
		friend class Gizmo;
		friend class SubTextureEditor;
	};
}