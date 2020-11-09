#include "SceneHierarchyPanel.h"
#include "Brbanje/Scene/Components.h"
#include "Brbanje/Scene/Entity.h"
#include "Brbanje/Core/Core.h"
#include "Brbanje/Core/Log.h"
#include "glm/gtc/type_ptr.hpp"
#include <imgui.h>

namespace Brbanje
{
	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene> scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(Ref<Scene> contex)
	{
		m_Scene = contex;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Scene->m_Registry.each([&](auto entityID) {

			Entity entity(entityID, m_Scene.get());
			DrawEntityNode(entity);

			});
		
		
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
		{
			m_EntitySelectionContext = Entity();
			BR_TRACE("krafna");
		}

		ImGui::End();

		

		ImGui::Begin("Properties");
		
		
		
		if (m_EntitySelectionContext)
		{
			DrawComponents(m_EntitySelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		std::string& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((entity == m_EntitySelectionContext) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_EntitySelectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}

	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				TransformComponent& transform = entity.GetComponent<TransformComponent>();
				ImGui::DragFloat3("Position", glm::value_ptr(transform.transform[3]), 0.5f);
				ImGui::TreePop();
			}
		}

	}
		

}