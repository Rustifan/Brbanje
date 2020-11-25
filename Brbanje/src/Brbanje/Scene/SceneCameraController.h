#pragma once

#include "SceneCamera.h"
#include "Components.h"
#include "Brbanje/Core/Timestep.h"
#include "Brbanje/Events/Event.h"
#include "Brbanje/Events/MouseEvents.h"
#include "glm/glm.hpp"

namespace Brbanje
{
	class Scene;

	class SceneCameraController
	{
	private:
		Scene* m_Scene;
		SceneCamera m_Camera;
		TransformComponent m_Transform;

		glm::vec3 m_StartCameraPos;
		glm::vec2 m_MouseStartPos;
		bool m_IsMoving = false;
		float m_MoveSpeed = 0.02f;
	private:
		void Move(Timestep ts);
	public:
		SceneCameraController(Scene* scene);
		~SceneCameraController() = default;
		void OnUpdate(Timestep ts);
		void OnRender();
		void OnImGuiRender();
		void OnEvent(Event& event);
		bool OnMouseScrolledEvent(MouseScrolledEvent& ev);
		SceneCamera* GetSceneCamera() { return &m_Camera; }
		TransformComponent* GetCameraTransform() { return &m_Transform; }
		
	};

}