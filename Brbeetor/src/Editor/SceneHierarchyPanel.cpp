#include "SceneHierarchyPanel.h"
#include "Brbanje/Scene/Components.h"
#include "Brbanje/Scene/Entity.h"
#include "Brbanje/Core/Core.h"
#include "Brbanje/Core/Log.h"
#include "glm/gtc/type_ptr.hpp"
#include <imgui.h>
#include "imgui_internal.h"

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
			
		}

		
		// add entity button	
		if (ImGui::BeginPopupContextWindow("add entity", ImGuiMouseButton_Right, false))
		{
			if (ImGui::MenuItem("add Entity"))
			{
				m_Scene->CreateEntity("new Entity");
				
			}
			ImGui::EndPopup();
			
		}

		
		


		
		
		ImGui::End();

		

		ImGui::Begin("Properties");
		
		

		
		if (m_EntitySelectionContext)
		{
			DrawComponents(m_EntitySelectionContext);

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("Add Component");
			}
			if (ImGui::BeginPopup("Add Component"))
			{
				if (ImGui::MenuItem("Sprite component"))
				{
					m_EntitySelectionContext.AddComponent<SpriteComponent>();
				}
				if (ImGui::MenuItem("Camera component"))
				{
					m_EntitySelectionContext.AddComponent<CameraComponent>();
				}
				ImGui::EndPopup();
			}
				
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		std::string& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((entity == m_EntitySelectionContext) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		bool isDeleted = false;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete entity"))
			{
				isDeleted = true;
			}

			ImGui::EndPopup();
		}

		if (ImGui::IsItemClicked())
		{
			m_EntitySelectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (isDeleted)
		{
			if (m_EntitySelectionContext == entity)
			{
				m_EntitySelectionContext = {};
			}
			m_Scene->DestroyEntity(entity);

		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		
		

		ImGui::PushID(label.c_str());
		
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		

		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight+3.0f, lineHeight };

		//Color Testing
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.9f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.2f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.1f, 0.15f, 1.0f));

		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
		}
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleColor(3);


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.5f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.15f, 1.0f));
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
		}
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);
		
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.15f, 0.7f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.6f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.15f, 0.7f, 1.0f));

		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
		}
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f,0.0f,0.0f,"%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		const ImGuiTreeNodeFlags componentNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
		
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
			
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), componentNodeFlags, "Transform"))
			{
				TransformComponent& transform = entity.GetComponent<TransformComponent>();
				
				
				

				DrawVec3Control("Position", transform.position);
				
				

				glm::vec3 rotationInDeg = glm::degrees(transform.rotation);
				DrawVec3Control("Rotation", rotationInDeg);
				transform.rotation = glm::radians(rotationInDeg);
				
				

				DrawVec3Control("Size", transform.size, 1.0f, 100.f);
			
				
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{

			bool open = (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), componentNodeFlags, "Camera"));
			
			ImGui::SameLine(ImGui::GetWindowSize().x - 35.0f);

			if (ImGui::Button("+"))
			{

				ImGui::OpenPopup("Remove Camera Component");
			}

			bool isDeleted = false;

			if (ImGui::BeginPopup("Remove Camera Component"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					isDeleted = true;
				}
				ImGui::EndPopup();
			}
			
			if (open)
			{
				CameraComponent& camera = entity.GetComponent<CameraComponent>();
				
				const char* previewValues[] = { "Perspective", "Orthographic" };
				const char* selectedValue = previewValues[(int)camera.camera.GetProjectionType()];

				ImGui::Checkbox("Primary", &camera.primary);

				if (ImGui::BeginCombo("Projection", selectedValue))
				{
					 

					for (uint32_t i = 0; i < 2; ++i)
					{
						bool isSelected = selectedValue == previewValues[i];
						if (ImGui::Selectable(previewValues[i], &isSelected))
						{
							selectedValue = previewValues[i];
							camera.camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}

					}
					ImGui::EndCombo();
				}

				if (camera.camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float size = camera.camera.GetSize();
					if (ImGui::DragFloat("size", &size, 0.1f))
					{
						camera.camera.SetSize(size);
					}
					
					
					float nearClip = camera.camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("near clip", &nearClip, 0.1f))
					{
						camera.camera.SetOrthoGraphicNearClip(nearClip);
					}

					float farClip = camera.camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("far clip", &farClip, 0.1f))
					{
						camera.camera.SetOrthoGraphicFarClip(farClip);
					}

					ImGui::Checkbox("Fixed Aspect Ratio", &camera.fixedAspectRatio);

				}

				else if (camera.camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float FOV = glm::degrees(camera.camera.GetPerspectiveFOV());
					if (ImGui::DragFloat("FOV", &FOV, 0.1f))
					{
						camera.camera.SetPerspectiveFOV(glm::radians(FOV));
					}

					float nearClip = camera.camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("near clip", &nearClip, 0.1f))
					{
						camera.camera.SetPerspectiveNearClip(nearClip);
					}

					float farClip = camera.camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("far clip", &farClip, 0.1f))
					{
						camera.camera.SetPerspectiveFarClip(farClip);
					}

				}

				ImGui::TreePop();
			}

			if (isDeleted)
			{
				m_EntitySelectionContext.RemoveComponent<CameraComponent>();
			}

		}

		if (entity.HasComponent<SpriteComponent>())
		{
			bool open = (ImGui::TreeNodeEx((void*)typeid(SpriteComponent).hash_code(), componentNodeFlags, "Sprite"));
			
			ImGui::SameLine(ImGui::GetWindowSize().x - 35.0f);

			if (ImGui::Button("+"))
			{
				
				ImGui::OpenPopup("Remove component");
			}

			bool isDeleted = false;

			if (ImGui::BeginPopup("Remove component"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					isDeleted = true;
				}
				ImGui::EndPopup();
			}

			if(open)
			{
				
				
				
				SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
				glm::vec4 color = sprite.color;
				if(ImGui::ColorEdit4("Color", (float*)&color))
				{
					sprite.color = color;
				}
				

				ImGui::TreePop();
			}

			if (isDeleted)
			{
				m_EntitySelectionContext.RemoveComponent<SpriteComponent>();
			}
		}

	}
		

}