#pragma once


#include "glm/glm.hpp"
#include "Entity.h"
#include "Brbanje/Core/Timestep.h"

namespace Brbanje
{
	

	class Gizmo
	{
	private:
		Entity m_Entity;
		glm::vec2 m_Size = { 0.2f,0.2f };
		glm::vec4 m_Color = { 0.2f,0.2f,0.7f,1.0f };
		float m_Alpha = 0.5f;
		float m_FadeSpeed = 0.2f;
		
		bool m_Move = false;
	public:
		Gizmo(Entity entity, glm::vec2 size = {0.2f,0.2f});
		Gizmo() = default;
		void SetEntity(Entity entity);
		void SetSize(const glm::vec2& size) { m_Size = size; }
		void OnRender();
		void OnUpdate(Timestep ts);
		void OnMove();
		bool isMoving() { return m_Move;	}
	};
}