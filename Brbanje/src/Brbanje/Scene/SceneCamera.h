#pragma once

#include "Brbanje/Renderer/Camera.h"

namespace Brbanje
{
	class SceneCamera : public Camera
	{
	private:
		uint32_t m_Width = 1080, m_Height = 720;
		float m_Size = 10.0f;
		float m_NearClip = -1.0f;
		float m_FarClip = 1.0f;
		float m_AspectRatio;
	private:
		void RecalculateProjectionMatrix();

	public:

		SceneCamera();
		virtual ~SceneCamera() {}
		void ResizeViewport(uint32_t width, uint32_t height);
		void SetOrthographic(float size, float near, float far);
		void SetSize(float size);
		float GetSize() { return m_Size; }

	};
}