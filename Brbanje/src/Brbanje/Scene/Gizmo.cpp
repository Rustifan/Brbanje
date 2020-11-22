#include "brpch.h"

#include "Gizmo.h"
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

		if (m_Scene->m_Panel->m_EntitySelectionContext && m_Scene->m_MainCamera)
		{
			float gizmoZ = m_Scene->m_MainCamera->GetOrthographicFarClip() - 0.01f;
			TransformComponent& transform = m_Scene->m_Panel->m_EntitySelectionContext.GetComponent<TransformComponent>();
			
			glm::mat4 cornerRotatedMat = glm::translate(glm::mat4(1.0f), transform.position) *
				glm::rotate(glm::mat4(1.0f), transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			
			glm::vec4 cornerA = { -transform.size.x / 2, -transform.size.y / 2, gizmoZ, 1.0f };
			glm::vec4 cornerB = { transform.size.x / 2, -transform.size.y / 2, gizmoZ, 1.0f };
			glm::vec4 cornerC = { transform.size.x / 2, transform.size.y / 2, gizmoZ, 1.0f };
			glm::vec4 cornerD = { -transform.size.x / 2, transform.size.y / 2, gizmoZ, 1.0f };
			
			glm::vec4 rotatedCornerA = cornerRotatedMat * cornerA;
			glm::vec4 rotatedCornerB = cornerRotatedMat * cornerB;
			glm::vec4 rotatedCornerC = cornerRotatedMat * cornerC;
			glm::vec4 rotatedCornerD = cornerRotatedMat * cornerD;
			std::array<glm::vec4*, 4> rotatedCorners = { &rotatedCornerD, &rotatedCornerC, &rotatedCornerB, &rotatedCornerA };


			Renderer2D::BeginScene(m_Scene->GetMainCamera()->GetProjection(), m_Scene->m_MainCameraTransform->GetTransform());
			
			//Middle Gizmo
			Renderer2D::DrawQuad(glm::vec3(transform.position.x, transform.position.y,
			m_Scene->GetMainCamera()->GetOrthographicFarClip() - 0.01f), m_Size, m_Color);


			//////////////W----I------P/////////////////////////////////////////////////////// Add functionaluity, transfer some code from render to update

		
			
			
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
			Renderer2D::DrawQuad(m_LeftResize.position, m_LeftResize.size, m_Color);
			
			//Right bar
			
			m_RightResize.position = { rightCor->x, transform.position.y,gizmoZ };
			m_RightResize.size = glm::vec2(m_ResizeSize, glm::abs(topCor->y-bottomCor->y));
			Renderer2D::DrawQuad(m_RightResize.position, m_RightResize.size, m_Color);
			
			//Top
			
			m_UpResize.position = { transform.position.x, topCor->y, gizmoZ };
			m_UpResize.size = glm::vec2(glm::abs(rightCor->x-leftCor->x), m_ResizeSize);
			Renderer2D::DrawQuad(m_UpResize.position, m_UpResize.size, m_Color);

			//Bottom
			
			m_DownResize.position = { transform.position.x, bottomCor->y, gizmoZ };
			m_DownResize.size = glm::vec2(glm::abs(rightCor->x - leftCor->x), m_ResizeSize);
			Renderer2D::DrawQuad(m_DownResize.position, m_DownResize.size, m_Color);

			//Resize Corners
			//A
			m_ResizeCornerA.position = { m_DownResize.position.x - m_DownResize.size.x / 2, m_DownResize.position.y, gizmoZ };
			m_ResizeCornerA.size = m_Size;
			Renderer2D::DrawQuad(m_ResizeCornerA.position, m_ResizeCornerA.size, m_Color);
			//B
			m_ResizeCornerB.position = { m_DownResize.position.x + m_DownResize.size.x / 2, m_DownResize.position.y, gizmoZ };
			m_ResizeCornerB.size = m_Size;
			Renderer2D::DrawQuad(m_ResizeCornerB.position, m_ResizeCornerB.size, m_Color);
			//C
			m_ResizeCornerC.position = { m_UpResize.position.x + m_UpResize.size.x / 2, m_UpResize.position.y, gizmoZ };
			m_ResizeCornerC.size = m_Size;
			Renderer2D::DrawQuad(m_ResizeCornerC.position, m_ResizeCornerC.size, m_Color);
			//D
			m_ResizeCornerD.position = { m_UpResize.position.x - m_UpResize.size.x / 2, m_UpResize.position.y, gizmoZ };
			m_ResizeCornerD.size = m_Size;
			Renderer2D::DrawQuad(m_ResizeCornerD.position, m_ResizeCornerD.size, m_Color);



			Renderer2D::EndScene();
		}
		

	}

	void Gizmo::OnUpdate(Timestep ts)
	{
		BR_PROFILE_FUNCTION;

		if (m_ColorChange  >= 0.8f )
		{
			if (m_FadeSpeed > 0)
			{
				m_FadeSpeed = -m_FadeSpeed;
			}
		}
		else if (m_ColorChange <= 0.3f)
		{
			if (m_FadeSpeed < 0 )
			{
				m_FadeSpeed = -m_FadeSpeed;
			}
		}

		m_ColorChange += m_FadeSpeed * ts;
		m_Color = { m_Color.x, m_Color.y, m_ColorChange, 1.0f };
		

		if (Input::IsMouseButtonPressed(0))
		{
			if (!m_Move)
			{
				TransformComponent& transform = m_Scene->m_Panel->m_EntitySelectionContext.GetComponent<TransformComponent>();
				glm::vec2 mousePos = m_Scene->GetSceneMousePos();
				if (mousePos.x > transform.position.x - m_Size.x / 2 &&
					mousePos.x < transform.position.x + m_Size.x / 2 &&
					mousePos.y > transform.position.y - m_Size.y / 2 &&
					mousePos.y < transform.position.y + m_Size.y / 2)
				{
					m_Move = true;
				}
			}
			
		}
		else
		{
			m_Move = false;
		}
		
		
		
	}

	void Gizmo::OnMove()
	{
		BR_PROFILE_FUNCTION;
		if (m_Move)
		{
			TransformComponent& transform = m_Scene->m_Panel->m_EntitySelectionContext.GetComponent<TransformComponent>();

			glm::vec2 mousePos = m_Scene->GetSceneMousePos();

			transform.position = glm::vec3(mousePos.x, mousePos.y, transform.position.z);
		}
	}

}