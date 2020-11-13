#pragma once

#include "Brbanje/Core/Core.h"
#include "Brbanje/Scene/Entity.h"

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
		friend class Scene;
	};
}