#pragma once


#include "glm/glm.hpp"
#include "Entity.h"
#include "Brbanje/Core/Timestep.h"

namespace Brbanje
{
	

	class Gizmo
	{
	private:
		
		Scene* m_Scene;
		glm::vec2 m_Size = { 0.2f,0.2f };
		glm::vec4 m_Color = { 0.2f,0.2f,0.7f,1.0f };
		float m_ColorChange = 0.5f;
		float m_FadeSpeed = 0.5f;
		bool m_MouseClicked = false;
		bool m_Move = false;
		bool m_LeftResizing = false, m_RightResizing = false, m_UpResizing = false, m_DownResizing = false;
		bool m_AResizing = false, m_BResizing = false, m_CResizing = false, m_DResizing = false;

		struct ResizeGizmo
		{
			glm::vec3 position;
			glm::vec2 size;
		};
		ResizeGizmo m_LeftResize, m_RightResize, m_DownResize, m_UpResize;
		ResizeGizmo m_ResizeCornerA, m_ResizeCornerB, m_ResizeCornerC, m_ResizeCornerD;

		float m_ResizingXStartPos = 0, m_ResizingYStartPos = 0;
		float m_XStartSize = 0, m_YStartSize = 0;
		float m_ResizeSize = 0.05f;
	public:
		Gizmo(Scene* scene, glm::vec2 size = {0.2f,0.2f});
		Gizmo() = default;
		
		void SetSize(const glm::vec2& size) { m_Size = size; }
		void OnRender();
		void OnUpdate(Timestep ts);
		void OnMove();
		bool isMoving() { return m_Move;	}
		bool isWorking() { return m_Move || m_LeftResizing || m_RightResizing || m_UpResizing || m_DownResizing || m_AResizing || m_BResizing || m_CResizing || m_DResizing; }
	};
}