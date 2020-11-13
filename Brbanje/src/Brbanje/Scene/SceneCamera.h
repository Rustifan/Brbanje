#pragma once

#include "Brbanje/Renderer/Camera.h"

namespace Brbanje
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	private:

		uint32_t m_Width = 1080, m_Height = 720;

		float m_Size = 10.0f;
		float m_OrthographicNearClip = -1.0f;
		float m_OrthographicFarClip = 1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNearClip = 0.01f;
		float m_PerspectiveFarClip = 1000.0f;


		float m_AspectRatio;
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;
	

	public:

		SceneCamera();
		virtual ~SceneCamera() {}
		void ResizeViewport(uint32_t width, uint32_t height);
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float FOV, float nearClip, float farClip);

		void SetSize(float size);
		float GetSize() { return m_Size; }
		ProjectionType GetProjectionType() { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjectionMatrix(); }

		float GetOrthographicNearClip()const { return m_OrthographicNearClip; }
		float GetOrthographicFarClip()const { return m_OrthographicFarClip; }
		void SetOrthoGraphicNearClip(float nearClip) { m_OrthographicNearClip = nearClip; RecalculateProjectionMatrix(); }
		void SetOrthoGraphicFarClip(float farClip) { m_OrthographicFarClip = farClip; RecalculateProjectionMatrix(); }


		float GetPerspectiveFOV()const { return m_PerspectiveFOV; }
		void SetPerspectiveFOV(float FOV) { m_PerspectiveFOV = FOV; RecalculateProjectionMatrix();}
		float GetPerspectiveNearClip()const { return m_PerspectiveNearClip; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNearClip = nearClip; RecalculateProjectionMatrix();}
		float GetPerspectiveFarClip() { return m_PerspectiveFarClip; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFarClip = farClip; RecalculateProjectionMatrix();}
		void RecalculateProjectionMatrix();

	};
}