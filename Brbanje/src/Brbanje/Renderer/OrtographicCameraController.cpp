#include "brpch.h"
#include "OrtographicCameraController.h"
#include "Brbanje/Core/KeyCodes.h"
#include "Brbanje/Core/Input.h"

namespace Brbanje
{
	OrtographicCameraController::OrtographicCameraController(float aspectRatio, bool rotation):
		m_AspectRatio(aspectRatio), m_Rotation(rotation),
		m_Bounds({ -m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom }),
		m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top)
	{
		BR_PROFILE_FUNCTION;
	}

	void OrtographicCameraController::OnUpdate(Timestep ts)
	{
		BR_PROFILE_FUNCTION;
		
		if (Input::IsKeyPressed(BR_KEY_A))
		{
			m_CameraPosition.x -= m_CameraSpeed * ts;
		}
		else if (Input::IsKeyPressed(BR_KEY_D))
		{
			m_CameraPosition.x += m_CameraSpeed * ts;
		}
		if (Input::IsKeyPressed(BR_KEY_W))
		{
			m_CameraPosition.y += m_CameraSpeed * ts;
		}
		else if (Input::IsKeyPressed(BR_KEY_S))
		{
			m_CameraPosition.y -= m_CameraSpeed * ts;
		}
		m_Camera.SetPosition(m_CameraPosition);
		
		if (m_Rotation)
		{

			if (Input::IsKeyPressed(BR_KEY_Q))
			{
				m_CameraRotation += m_RotationSpeed * ts;
			}
			else if (Input::IsKeyPressed(BR_KEY_E))
			{
				m_CameraRotation -= m_RotationSpeed * ts;
			}
			m_Camera.SetRotiation(m_CameraRotation);
		}
	}

	void OrtographicCameraController::OnEvent(Event& e)
	{
		BR_PROFILE_FUNCTION;
		
		EventDispacher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrtographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrtographicCameraController::OnWindowResize));

	}

	bool OrtographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		BR_PROFILE_FUNCTION;
		
		m_Zoom -= e.GetYOffset() * 0.25f;
		m_Zoom = std::max(m_Zoom,0.25f);
		m_CameraSpeed = m_Zoom;
		m_Bounds = { -m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	
		
		return false;
	}

	bool OrtographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		BR_PROFILE_FUNCTION;

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Bounds = { -m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom };
		m_Camera.SetProjection(m_Bounds.Left,m_Bounds.Right,m_Bounds.Bottom, m_Bounds.Top);
		return false;

	}
}