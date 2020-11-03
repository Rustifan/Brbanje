#pragma once

#include "Brbanje/Renderer/OrthographicCamera.h"
#include "Brbanje/Core/Timestep.h"

#include "Brbanje/Events/ApplicationEvent.h"
#include "Brbanje/Events/MouseEvents.h"

namespace Brbanje
{
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrtographicCameraController
	{
	private:
		float m_Zoom = 1.0f;
		float m_AspectRatio;
		bool m_Rotation = false;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;
		
		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraSpeed = 5.0f, m_RotationSpeed = 160.0f;
	private:
		void CalculateProjection();
	public:
		OrtographicCameraController(float aspectRatio, bool rotation = false);
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		void SetZoom(float zoom) { m_Zoom = zoom; CalculateProjection(); }
		float GetZoom()const { return m_Zoom; }

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCameraBounds& GetBounds()const{ return m_Bounds; }

	};
}