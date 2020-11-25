#include "brpch.h"


#include "Gizmo.h"
#include "imgui.h"

#include "Entity.h"
#include "Scene.h"
#include "Brbanje/Renderer/Renderer2D.h"
#include "Components.h"
#include "Brbanje/Core/Input.h"
#include "../../Brbeetor/src/Editor/GizmoLayer.h"

namespace Brbanje
{
	
	
	Gizmo::Gizmo(Scene* scene, glm::vec2 size) :m_Scene(scene), m_Size(size)
	{
		
	}

	

	void Gizmo::OnRender()
	{
		BR_PROFILE_FUNCTION;

		if (m_Scene->m_Panel->m_EntitySelectionContext && m_Scene->m_MainCamera && m_Scene->m_MainCamera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
		{
			TransformComponent& transform = m_Scene->m_Panel->m_EntitySelectionContext.GetComponent<TransformComponent>();
			
		

			Renderer2D::BeginScene(m_Scene->GetMainCamera()->GetProjection(), m_Scene->m_MainCameraTransform->GetTransform());
			
			//Middle Gizmo
			Renderer2D::DrawQuad(glm::vec3(transform.position.x, transform.position.y,
			m_Scene->GetMainCamera()->GetOrthographicFarClip() - 0.01f), m_Size, m_Color);


			
			//Resizing Update

			float gizmoZ = m_Scene->m_MainCamera->GetOrthographicFarClip() - 0.01f;
			glm::mat4 cornerRotatedMat = glm::translate(glm::mat4(1.0f), transform.position) *
				glm::rotate(glm::mat4(1.0f), transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			glm::vec4 cornerA = { -glm::abs(transform.size.x / 2), -glm::abs(transform.size.y/ 2), gizmoZ, 1.0f };
			glm::vec4 cornerB = { glm::abs(transform.size.x / 2), -glm::abs(transform.size.y / 2), gizmoZ, 1.0f };
			glm::vec4 cornerC = { glm::abs(transform.size.x / 2), glm::abs(transform.size.y / 2), gizmoZ, 1.0f };
			glm::vec4 cornerD = { -glm::abs(transform.size.x / 2), glm::abs(transform.size.y / 2), gizmoZ, 1.0f };

			glm::vec4 rotatedCornerA = cornerRotatedMat * cornerA;
			glm::vec4 rotatedCornerB = cornerRotatedMat * cornerB;
			glm::vec4 rotatedCornerC = cornerRotatedMat * cornerC;
			glm::vec4 rotatedCornerD = cornerRotatedMat * cornerD;
			std::array<glm::vec4*, 4> rotatedCorners = { &rotatedCornerD, &rotatedCornerC, &rotatedCornerB, &rotatedCornerA };

			//Corner pointers
			int leftIndex, rightIndex, upIndex, downIndex;

			if (transform.rotation.z >= 0)
			{
				leftIndex = (int)((glm::degrees(transform.rotation.z) / 90.0f)) % 4;
				rightIndex = (int)((glm::degrees(transform.rotation.z) / 90.0f) + 2) % 4;
				upIndex = (int)((glm::degrees(transform.rotation.z) / 90.0f) + 1) % 4;
				downIndex = (int)((glm::degrees(transform.rotation.z) / 90.0f) + 3) % 4;


			}
			else
			{
				leftIndex = 3 - (int)(glm::abs(glm::degrees(transform.rotation.z)) / 90.0f) % 4;
				rightIndex = 3 - (int)((glm::abs(glm::degrees(transform.rotation.z)) / 90.0f) + 2) % 4;
				upIndex = 3 - (int)((glm::abs(glm::degrees(transform.rotation.z)) / 90.0f) + 3) % 4;
				downIndex = 3 - (int)((glm::abs(glm::degrees(transform.rotation.z)) / 90.0f) + 1) % 4;
			}

			glm::vec4* leftCor = rotatedCorners[leftIndex];
			glm::vec4* rightCor = rotatedCorners[rightIndex];
			glm::vec4* topCor = rotatedCorners[upIndex];
			glm::vec4* bottomCor = rotatedCorners[downIndex];

			//Bars
			//Left bar
			m_LeftResize.position = glm::vec3(leftCor->x, transform.position.y, gizmoZ);
			m_LeftResize.size = glm::vec2(m_ResizeSize, glm::abs(topCor->y - bottomCor->y));

			//Right bar

			m_RightResize.position = { rightCor->x, transform.position.y,gizmoZ };
			m_RightResize.size = glm::vec2(m_ResizeSize, glm::abs(topCor->y - bottomCor->y));

			//Top

			m_UpResize.position = { transform.position.x, topCor->y, gizmoZ };
			m_UpResize.size = glm::vec2(glm::abs(rightCor->x - leftCor->x), m_ResizeSize);

			//Bottom

			m_DownResize.position = { transform.position.x, bottomCor->y, gizmoZ };
			m_DownResize.size = glm::vec2(glm::abs(rightCor->x - leftCor->x), m_ResizeSize);

			//Resize Corners
			//A
			m_ResizeCornerA.position = { m_DownResize.position.x - m_DownResize.size.x / 2, m_DownResize.position.y, gizmoZ };
			m_ResizeCornerA.size = m_Size;
			//B
			m_ResizeCornerB.position = { m_DownResize.position.x + m_DownResize.size.x / 2, m_DownResize.position.y, gizmoZ };
			m_ResizeCornerB.size = m_Size;
			//C
			m_ResizeCornerC.position = { m_UpResize.position.x + m_UpResize.size.x / 2, m_UpResize.position.y, gizmoZ };
			m_ResizeCornerC.size = m_Size;
			//D
			m_ResizeCornerD.position = { m_UpResize.position.x - m_UpResize.size.x / 2, m_UpResize.position.y, gizmoZ };
			m_ResizeCornerD.size = m_Size;
			
			
			//Drawing resize bars
			Renderer2D::DrawQuad(m_LeftResize.position, m_LeftResize.size, m_Color);
			Renderer2D::DrawQuad(m_RightResize.position, m_RightResize.size, m_Color);
			Renderer2D::DrawQuad(m_UpResize.position, m_UpResize.size, m_Color);
			Renderer2D::DrawQuad(m_DownResize.position, m_DownResize.size, m_Color);

			//Drawind resize corners
			Renderer2D::DrawQuad(m_ResizeCornerA.position, m_ResizeCornerA.size, m_Color);
			Renderer2D::DrawQuad(m_ResizeCornerB.position, m_ResizeCornerB.size, m_Color);
			Renderer2D::DrawQuad(m_ResizeCornerC.position, m_ResizeCornerC.size, m_Color);
			Renderer2D::DrawQuad(m_ResizeCornerD.position, m_ResizeCornerD.size, m_Color);

			
			
			



			Renderer2D::EndScene();
		}
		

	}

	void Gizmo::OnUpdate(Timestep ts)
	{
		BR_PROFILE_FUNCTION;
		if (m_Scene->m_Panel->m_EntitySelectionContext && m_Scene->m_MainCamera)
		{
			TransformComponent& transform = m_Scene->m_Panel->m_EntitySelectionContext.GetComponent<TransformComponent>();
			
			m_Size = m_Scene->m_MainCamera->GetSize() * 0.1f * glm::vec2(0.2f, 0.2f);
			m_ResizeSize = m_Scene->m_MainCamera->GetSize() * 0.1f * 0.05f;

			//Fade Speed control
			if (m_ColorChange >= 0.8f)
			{
				if (m_FadeSpeed > 0)
				{
					m_FadeSpeed = -m_FadeSpeed;
				}
			}
			else if (m_ColorChange <= 0.3f)
			{
				if (m_FadeSpeed < 0)
				{
					m_FadeSpeed = -m_FadeSpeed;
				}
			}

			m_ColorChange += m_FadeSpeed * ts;
			m_Color = { m_Color.x, m_Color.y, m_ColorChange, 1.0f };


			//Moving Gizmo Click
			
			
				if (!m_Move)
				{
					
					

					glm::vec2 mousePos = m_Scene->GetSceneMousePos();
					if (mousePos.x > transform.position.x - m_Size.x / 2 &&
						mousePos.x < transform.position.x + m_Size.x / 2 &&
						mousePos.y > transform.position.y - m_Size.y / 2 &&
						mousePos.y < transform.position.y + m_Size.y / 2 
						)
					{
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
						if (ImGui::IsMouseClicked(0))
						{
							m_Move = true;
						}
					}
				}

			
			//Resizing
			
			if (m_Scene->IsClicked(m_ResizeCornerA.position, m_ResizeCornerA.size, m_Scene->GetSceneMousePos()))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNESW);
				if (ImGui::IsMouseClicked(0))
				{
					m_AResizing = true;
					m_ResizingXStartPos = m_Scene->GetSceneMousePos().x;
					m_ResizingYStartPos = m_Scene->GetSceneMousePos().y;
					m_XStartSize = transform.size.x;
					m_YStartSize = transform.size.y;
					
				}

			}
			else if (m_Scene->IsClicked(m_ResizeCornerB.position, m_ResizeCornerB.size, m_Scene->GetSceneMousePos()))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNWSE);
				if (ImGui::IsMouseClicked(0))
				{
					m_BResizing = true;
					m_ResizingXStartPos = m_Scene->GetSceneMousePos().x;
					m_ResizingYStartPos = m_Scene->GetSceneMousePos().y;
					m_XStartSize = transform.size.x;
					m_YStartSize = transform.size.y;

				}
			}
			else if (m_Scene->IsClicked(m_ResizeCornerC.position, m_ResizeCornerC.size, m_Scene->GetSceneMousePos()))
			{	
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNESW);
				if (ImGui::IsMouseClicked(0))
				{
					m_CResizing = true;
					m_ResizingXStartPos = m_Scene->GetSceneMousePos().x;
					m_ResizingYStartPos = m_Scene->GetSceneMousePos().y;
					m_XStartSize = transform.size.x;
					m_YStartSize = transform.size.y;

				}
			}
			else if (m_Scene->IsClicked(m_ResizeCornerD.position, m_ResizeCornerD.size, m_Scene->GetSceneMousePos()))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNWSE);
				if (ImGui::IsMouseClicked(0))
				{
					m_DResizing = true;
					m_ResizingXStartPos = m_Scene->GetSceneMousePos().x;
					m_ResizingYStartPos = m_Scene->GetSceneMousePos().y;
					m_XStartSize = transform.size.x;
					m_YStartSize = transform.size.y;

				}
			}
			else if (m_Scene->IsClicked(m_LeftResize.position, m_LeftResize.size, m_Scene->GetSceneMousePos()))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
				if (ImGui::IsMouseClicked(0))
				{
					
					m_LeftResizing = true;
					m_ResizingXStartPos = m_Scene->GetSceneMousePos().x;
					m_XStartSize = transform.size.x;
					m_YStartSize = transform.size.y;
					
				}
			}
			else if (m_Scene->IsClicked(m_RightResize.position, m_RightResize.size, m_Scene->GetSceneMousePos()))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
				if (ImGui::IsMouseClicked(0))
				{

					m_RightResizing = true;
					m_ResizingXStartPos = m_Scene->GetSceneMousePos().x;
					m_XStartSize = transform.size.x;
					m_YStartSize = transform.size.y;
				} 

			}
			else if (m_Scene->IsClicked(m_UpResize.position, m_UpResize.size, m_Scene->GetSceneMousePos()))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
				if (ImGui::IsMouseClicked(0))
				{

					m_UpResizing = true;
					m_ResizingYStartPos = m_Scene->GetSceneMousePos().y;
					m_XStartSize = transform.size.x;
					m_YStartSize = transform.size.y;
				}

			}
			else if (m_Scene->IsClicked(m_DownResize.position, m_DownResize.size, m_Scene->GetSceneMousePos()))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
				if (ImGui::IsMouseClicked(0))
				{

					m_DownResizing = true;
					m_ResizingYStartPos = m_Scene->GetSceneMousePos().y;
					m_XStartSize = transform.size.x;
					m_YStartSize = transform.size.y;
				}
			}
			


			if (ImGui::IsMouseReleased(0))
			{
				
				m_Move = false;
				m_LeftResizing = false;
				m_RightResizing = false;
				m_UpResizing = false;
				m_DownResizing = false;
				m_AResizing = false;
				m_BResizing = false;
				m_CResizing = false;
				m_DResizing = false;
			}
		}
	}

	void Gizmo::OnMove()
	{
		BR_PROFILE_FUNCTION;
		if (m_Scene->m_Panel->m_EntitySelectionContext && m_Scene->m_MainCamera)
		{

			TransformComponent& transform = m_Scene->m_Panel->m_EntitySelectionContext.GetComponent<TransformComponent>();
			int rotation = (int)(glm::degrees(transform.rotation.z));
			rotation = rotation % 360;
			if (rotation < 0) rotation = 360 + rotation;
			

			if (m_Move)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				glm::vec2 mousePos = m_Scene->GetSceneMousePos();

				transform.position = glm::vec3(mousePos.x, mousePos.y, transform.position.z);
			}
			else if (m_AResizing)
			{
				if (rotation > 315 || rotation <= 45 || (rotation > 135 && rotation <= 225))
				{
					transform.size.x = m_XStartSize + m_ResizingXStartPos - m_Scene->GetSceneMousePos().x;
					transform.size.y = m_YStartSize + m_ResizingYStartPos - m_Scene->GetSceneMousePos().y;
				}
				else if ((rotation > 45 && rotation <= 135) || (rotation > 225 && rotation <= 315))
				{
					transform.size.y = m_YStartSize + m_ResizingXStartPos - m_Scene->GetSceneMousePos().x;
					transform.size.x = m_XStartSize + m_ResizingYStartPos - m_Scene->GetSceneMousePos().y;
				}

				
			}
			else if (m_BResizing)
			{
				if (rotation > 315 || rotation <= 45 || (rotation > 135 && rotation <= 225))
				{
					transform.size.x = m_XStartSize + m_Scene->GetSceneMousePos().x - m_ResizingXStartPos;
					transform.size.y = m_YStartSize + m_ResizingYStartPos - m_Scene->GetSceneMousePos().y;
				}
				else if ((rotation > 45 && rotation <= 135) || (rotation > 225 && rotation <= 315))
				{
					transform.size.y = m_YStartSize + m_Scene->GetSceneMousePos().x - m_ResizingXStartPos;
					transform.size.x = m_XStartSize + m_ResizingYStartPos - m_Scene->GetSceneMousePos().y;
				}
			}
			else if (m_CResizing)
			{
				if (rotation > 315 || rotation <= 45 || (rotation > 135 && rotation <= 225))
				{
					transform.size.x = m_XStartSize + m_Scene->GetSceneMousePos().x - m_ResizingXStartPos;
					transform.size.y = m_YStartSize + m_Scene->GetSceneMousePos().y - m_ResizingYStartPos;
				}
				else if ((rotation > 45 && rotation <= 135) || (rotation > 225 && rotation <= 315))
				{
					transform.size.y = m_YStartSize + m_Scene->GetSceneMousePos().x - m_ResizingXStartPos;
					transform.size.x = m_XStartSize + m_Scene->GetSceneMousePos().y - m_ResizingYStartPos;
				}
			}
			else if (m_DResizing)
			{
				if (rotation > 315 || rotation <= 45 || (rotation > 135 && rotation <= 225))
				{
					transform.size.x = m_XStartSize + m_ResizingXStartPos - m_Scene->GetSceneMousePos().x;
					transform.size.y = m_YStartSize + m_Scene->GetSceneMousePos().y - m_ResizingYStartPos;
				}
				else if ((rotation > 45 && rotation <= 135) || (rotation > 225 && rotation <= 315))
				{
					transform.size.y = m_YStartSize + m_ResizingXStartPos - m_Scene->GetSceneMousePos().x;
					transform.size.x = m_XStartSize + m_Scene->GetSceneMousePos().y - m_ResizingYStartPos;
				}
			}
			else if (m_LeftResizing)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
				if (rotation > 315  || rotation <= 45 || (rotation > 135 && rotation <= 225))
				{
					transform.size.x = m_XStartSize + m_ResizingXStartPos - m_Scene->GetSceneMousePos().x;
				}
				else if ((rotation > 45 && rotation <= 135) || (rotation > 225 && rotation <= 315))
				{
					transform.size.y = m_YStartSize + m_ResizingXStartPos - m_Scene->GetSceneMousePos().x;
				}
				
			}
			else if(m_RightResizing)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
				
				if (rotation > 315 || rotation <= 45 || (rotation > 135 && rotation <= 225))
				{
					transform.size.x = m_XStartSize + m_Scene->GetSceneMousePos().x - m_ResizingXStartPos;
				}
				else if ((rotation > 45 && rotation <= 135) || (rotation > 225 && rotation <= 315))
				{
					transform.size.y = m_YStartSize + m_Scene->GetSceneMousePos().x - m_ResizingXStartPos;
				}
			}
			else if (m_UpResizing)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);

				if (rotation > 315 || rotation <= 45 || (rotation > 135 && rotation <= 225))
				{
					transform.size.y = m_YStartSize + m_Scene->GetSceneMousePos().y - m_ResizingYStartPos;
				}
				else if ((rotation > 45 && rotation <= 135) || (rotation > 225 && rotation <= 315))
				{
					transform.size.x = m_XStartSize + m_Scene->GetSceneMousePos().y - m_ResizingYStartPos;
				}

			}
			else if (m_DownResizing)
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
				
				if (rotation > 315 || rotation <= 45 || (rotation > 135 && rotation <= 225))
				{
					transform.size.y = m_YStartSize + m_ResizingYStartPos - m_Scene->GetSceneMousePos().y;
				}
				else if ((rotation > 45 && rotation <= 135) || (rotation > 225 && rotation <= 315))
				{
					transform.size.x = m_XStartSize + m_ResizingYStartPos - m_Scene->GetSceneMousePos().y;
				}
			}

		}
	}

}