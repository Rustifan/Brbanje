#include "brpch.h"
#include "SceneCamera.h"

namespace Brbanje
{
	
	SceneCamera::SceneCamera(): m_AspectRatio((float)m_Width / (float)m_Height)
	{
		RecalculateProjectionMatrix();
	}
	
	void SceneCamera::ResizeViewport(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_Size = size;
		m_OrthographicNearClip = nearClip;
		m_OrthographicFarClip = farClip;

		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetPerspective(float FOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = FOV;
		m_PerspectiveNearClip = nearClip;
		m_PerspectiveFarClip = farClip;

		RecalculateProjectionMatrix();
	}

	void SceneCamera::RecalculateProjectionMatrix()
	{
		m_AspectRatio = (float)m_Width / (float)m_Height;
		
		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNearClip, m_PerspectiveFarClip);
			break;
		case ProjectionType::Orthographic:

			float left = -m_AspectRatio * m_Size * 0.5f;
			float right = m_AspectRatio * m_Size * 0.5f;
			float down = -m_Size * 0.5f;
			float up = m_Size * 0.5f;
			m_Projection = glm::ortho(left, right, down, up, m_OrthographicNearClip, m_OrthographicFarClip);
			break;
		}

		
	}

	void SceneCamera::SetSize(float size)
	{
		m_Size = size;
		RecalculateProjectionMatrix();
	}


	
}