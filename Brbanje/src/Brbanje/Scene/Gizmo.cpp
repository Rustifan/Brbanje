#include "brpch.h"

#include "Gizmo.h"
#include "Entity.h"
#include "Scene.h"
#include "Brbanje/Renderer/Renderer2D.h"
#include "Components.h"
#include "Brbanje/Core/Input.h"

namespace Brbanje
{
	static TransformComponent* transform = nullptr;
	
	Gizmo::Gizmo(Entity entity, glm::vec2 size) :m_Entity(entity), m_Size(size)
	{

	}

	void Gizmo::SetEntity(Entity entity)
	{
		m_Entity = entity;
		transform = &m_Entity.GetComponent<TransformComponent>();
	}

	void Gizmo::OnRender()
	{
		
		
		Renderer2D::DrawQuad(glm::vec3(transform->position.x, transform->position.y, 
			m_Entity.GetScene()->GetMainCamera()->GetOrthographicFarClip()-0.01f), m_Size, m_Color);

	}

	void Gizmo::OnUpdate(Timestep ts)
	{
		if (m_Alpha  >= 0.8f || m_Alpha  <= 0.5f)
		{
			m_FadeSpeed = -m_FadeSpeed;
		}
		m_Alpha += m_FadeSpeed * ts;
		m_Color = { m_Color.x, m_Color.y, m_Color.z, m_Alpha };

		if (Input::IsMouseButtonPressed(0))
		{
			if (!m_Move)
			{
				glm::vec2 mousePos = m_Entity.GetScene()->GetSceneMousePos();
				if (mousePos.x > transform->position.x - m_Size.x / 2 &&
					mousePos.x < transform->position.x + m_Size.x / 2 &&
					mousePos.y > transform->position.y - m_Size.y / 2 &&
					mousePos.y < transform->position.y + m_Size.y / 2)
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
		if (m_Move)
		{
			glm::vec2 mousePos = m_Entity.GetScene()->GetSceneMousePos();

			transform->position = glm::vec3(mousePos.x, mousePos.y, transform->position.z);
		}
	}

}