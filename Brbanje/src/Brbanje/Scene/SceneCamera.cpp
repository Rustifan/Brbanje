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
		m_Size = size;
		m_NearClip = nearClip;
		m_FarClip = farClip;

		RecalculateProjectionMatrix();
	}

	void SceneCamera::RecalculateProjectionMatrix()
	{
		m_AspectRatio = (float)m_Width / (float)m_Height;
		float left = -m_AspectRatio * m_Size * 0.5f;
		float right = m_AspectRatio * m_Size * 0.5f;
		float down = -m_Size * 0.5f;
		float up = m_Size * 0.5f;
		m_Projection = glm::ortho(left, right, down, up, m_NearClip, m_FarClip);
	}

	void SceneCamera::SetSize(float size)
	{
		m_Size = size;
		RecalculateProjectionMatrix();
	}

	
}