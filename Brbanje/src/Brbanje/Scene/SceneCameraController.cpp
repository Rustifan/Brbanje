#include "brpch.h"
#include "SceneCameraController.h"
#include "Brbanje/Core/Input.h"
#include "Brbanje/Events/MouseEvents.h"
#include "Scene.h"
#include "imgui.h"

namespace Brbanje
{
	SceneCameraController::SceneCameraController(Scene* scene):
		m_Scene(scene), m_Camera(SceneCamera()), m_Transform(TransformComponent())
	{
		m_MoveSpeed = m_Camera.GetSize() * 0.001f;
	}

	void SceneCameraController::OnUpdate(Timestep ts)
	{
		
		
		if (ImGui::IsMouseClicked(1) && m_Scene->IsMouseHovered())
		{
			ImGuiIO& io = ImGui::GetIO();

			m_IsMoving = true;
			m_StartCameraPos = m_Transform.position;
			m_MouseStartPos = glm::vec2(io.MousePos.x, io.MousePos.y);
		}
		if (ImGui::IsMouseReleased(1))
		{
			m_IsMoving = false;
		}

		Move(ts);

	}

	void SceneCameraController::OnRender()
	{

	}

	void SceneCameraController::OnImGuiRender()
	{
		ImGui::Begin("Editor Camera Settings");

		ImGui::Dummy(ImVec2(0, 5.0f));


		SceneHierarchyPanel::DrawVec3Control("Position", m_Transform.position);

		glm::vec3 rotation = glm::degrees(m_Transform.rotation);
		SceneHierarchyPanel::DrawVec3Control("Rotation", rotation, 0.0f);
		m_Transform.rotation = glm::radians(rotation);
		

		ImGui::Dummy(ImVec2(0, 10.0f));

		float size = m_Camera.GetSize();
		if (ImGui::DragFloat("size", &size, 0.1f))
		{
			m_Camera.SetSize(size);
		}


		float nearClip = m_Camera.GetOrthographicNearClip();
		if (ImGui::DragFloat("near clip", &nearClip, 0.1f))
		{
			m_Camera.SetOrthoGraphicNearClip(nearClip);
		}

		float farClip = m_Camera.GetOrthographicFarClip();
		if (ImGui::DragFloat("far clip", &farClip, 0.1f))
		{
			m_Camera.SetOrthoGraphicFarClip(farClip);
		}



		ImGui::End();
	}

	void SceneCameraController::OnEvent(Event& event)
	{
		EventDispacher ed(event);
		ed.Dispatch<MouseScrolledEvent>(std::bind(&SceneCameraController::OnMouseScrolledEvent, this, std::placeholders::_1));
	}

	bool SceneCameraController::OnMouseScrolledEvent(MouseScrolledEvent& ev)
	{
		if (m_Scene->IsMouseHovered())
		{
			m_Camera.SetSize(m_Camera.GetSize() - ev.GetYOffset());
			
			m_MoveSpeed = m_Camera.GetSize() * 0.001f;
		}

		return false;
	}

	void SceneCameraController::Move(Timestep ts)
	{
		if (m_IsMoving)
		{
			ImGuiIO& io = ImGui::GetIO();

			glm::vec2 mousePosDiff;
			mousePosDiff.y = io.MousePos.y - m_MouseStartPos.y;
			mousePosDiff.x = m_MouseStartPos.x - io.MousePos.x;

			m_Transform.position = m_StartCameraPos + glm::vec3(mousePosDiff.x, mousePosDiff.y, 0.0f) * m_MoveSpeed;

			
		}
	}

	
}