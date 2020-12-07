#include "SceneHierarchyPanel.h"
#include "Brbanje/Scene/Components.h"
#include "Brbanje/Scene/Entity.h"
#include "Brbanje/Core/Core.h"
#include "Brbanje/Core/Log.h"
#include  "Brbanje/Scene/Scene.h"
#include "Brbanje/Core/Input.h"
#include "glm/gtc/type_ptr.hpp"
#include <imgui.h>
#include "imgui_internal.h"
#include "Brbanje/Utils/FileDialog.h"

namespace Brbanje
{
	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene> scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(Ref<Scene> contex)
	{
		m_Scene = contex;
		m_Scene->m_Panel = this;
		
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
		}

		ImGui::End();

		m_Scene->m_CameraController.OnImGuiRender();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		std::string& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((entity == m_EntitySelectionContext) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanFullWidth;

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

	void SceneHierarchyPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
	{
		
		ImGuiIO& io = ImGui::GetIO();


		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());
		
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		

		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		
		float itemWidth = ImGui::CalcItemWidth();
		ImGui::PushMultiItemsWidths(3, itemWidth);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		//Color Testing
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
			
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();



		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f,0.0f,0.0f,"%.2f");
		ImGui::PopItemWidth();
		

		ImGui::Columns(1);
		ImGui::PopStyleVar();
		
		ImGui::PopID();
	}

	

	template<typename T, typename ComponentUIFunction>
	static void DrawComponent(const std::string& name, Entity entity, ComponentUIFunction compFunc)
	{
		if (entity.HasComponent<T>())
		{
			const ImGuiTreeNodeFlags componentNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed |
				ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
			
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			ImGui::Separator();

			bool open = (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), componentNodeFlags, name.c_str()));
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvail.x- lineHeight * 0.5f);

			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
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

			if (open)
			{
				T& component = entity.GetComponent<T>();

				compFunc(component);

				ImGui::TreePop();
			}

			if (isDeleted)
			{
				entity.RemoveComponent<T>();
			}
		}
	}


	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		const ImGuiTreeNodeFlags componentNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
		
		if (entity.HasComponent<TagComponent>())
		{
			

			auto& tag = entity.GetComponent<TagComponent>().tag;

			char buffer[100];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();

		ImGui::PushItemWidth(-1);
		
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component");
		}
		if (ImGui::BeginPopup("Add Component"))
		{
			
			if (!m_EntitySelectionContext.HasComponent<TransformComponent>())
			{
				if (ImGui::MenuItem("Transform component"))
				{
					m_EntitySelectionContext.AddComponent<TransformComponent>();
				}
			}
			
			
			if (!m_EntitySelectionContext.HasComponent<SpriteComponent>())
			{
				if (ImGui::MenuItem("Sprite component"))
				{
					m_EntitySelectionContext.AddComponent<SpriteComponent>();
				}
			}

			if (!m_EntitySelectionContext.HasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera component"))
				{
					m_EntitySelectionContext.AddComponent<CameraComponent>();
				}
			}
		

			
			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform Component", m_EntitySelectionContext, [this,&entity](TransformComponent& transform) {

			DrawVec3Control("Position", transform.position);



			glm::vec3 rotationInDeg = glm::degrees(transform.rotation);
			DrawVec3Control("Rotation", rotationInDeg);
			transform.rotation = glm::radians(rotationInDeg);



			DrawVec3Control("Size", transform.size, 1.0f, 100.f);

			

			});
		

		DrawComponent<CameraComponent>("Camera Component", m_EntitySelectionContext, [](CameraComponent& camera) {
			
			
			
				

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
			
			
			
			
			});
		
		
		
		DrawComponent<SpriteComponent>("Sprite Component", m_EntitySelectionContext, [this](SpriteComponent& sprite) {

			glm::vec4 color = sprite.color;
			if (ImGui::ColorEdit4("Color", (float*)&color))
			{
				sprite.color = color;
			}

			std::string text;
			if (sprite.texture)
			{
				text = sprite.texture->GetFilePath();
			}
			else
			{
				text = "No Texture";
			}
			char buffer[100];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), text.c_str());
			

			ImGui::InputText("##Texture", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);
			
				
				if (ImGui::BeginPopupContextItem("##Texture"))
				{
					if (ImGui::MenuItem("Remove texture"))
					{
						sprite.texture = {};
					}

					ImGui::EndPopup();
				}
				

					
				
				
				
				ImGui::SameLine();
				if (ImGui::Button("Set Texture"))
				{

					std::optional<std::string> TextureFilePath = FileDialog::OpenFile("Portable Network Graphics(*.png)\0*.png\0");
					if (TextureFilePath)
					{
						sprite.texture = m_Scene->GetTextureFromTextureMap(TextureFilePath.value());
						sprite.subTexture = nullptr;
					}


				}
				

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100);
			ImGui::Text("Tiling factor");
			
			ImGui::NextColumn();
			float tiling = sprite.tilingFactor;
			if (ImGui::DragFloat("##tiling factor", &tiling, 0.1f, 1.0f))
			{
				sprite.tilingFactor = tiling;
			}
			ImGui::Columns(1);
			
			if (sprite.texture)
			{
				if (ImGui::Button("Edit Texture"))
				{

					m_Scene->m_SubTexEditor.SetEdit(true);


				}
			}
			
			

			});
	}
		

}