#include "brpch.h"
#include "Scene.h"
#include "imgui.h"
#include "Brbanje/Renderer/Renderer2D.h"
#include "Entity.h"
#include "Brbanje/Scene/ScriptableEntity.h"
#include "Components.h"
#include "Brbanje/Core/Input.h"


namespace Brbanje
{
	Scene::Scene()
	{


	}

	Scene::~Scene()
	{
		
	}

	void Scene::OnUpdate(Timestep ts)
	{
		
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& component)
			{
				if (!component.instance)
				{
					component.instance = component.InstatiateFunction();
					component.instance->m_Entity = Entity(entity, this);
					component.instance->OnCreate();
					
					

					
				}

				component.instance->OnUpdate(ts);
			});

		
		m_MainCamera = nullptr;
		glm::mat4 transform;
		{
			auto group = m_Registry.group<>(entt::get<TransformComponent, CameraComponent>);
			for (auto entity : group)
			{
				auto [trasformComp, camera] = group.get<TransformComponent, CameraComponent>(entity);
				if (camera.primary)
				{
					m_MainCamera = &camera.camera;
					
					transform = trasformComp.GetTransform();
					m_MainCameraTransform = &trasformComp;
				}

			}
		}

		if (m_MainCamera)
		{

			Renderer2D::BeginScene(m_MainCamera->GetProjection(), transform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group)
			{
				if (m_Registry.has<SpriteComponent>(entity))
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

					
					if (Input::IsMouseButtonPressed(0))
					{

						
						
							glm::vec2 mousePos = GetSceneMousePos();
							if (mousePos.x > transform.position.x - transform.size.x / 2 &&
								mousePos.x < transform.position.x + transform.size.x / 2 &&
								mousePos.y > transform.position.y - transform.size.y / 2 &&
								mousePos.y < transform.position.y + transform.size.y / 2)
							{
								
								if (!m_Gizmo.isMoving())
								{
									m_Panel->m_EntitySelectionContext = Entity(entity, this);

								}
							

							}
						
					}
					
					Renderer2D::DrawQuad(transform.GetTransform(), sprite);
				}
				
			}

			if (m_Panel->m_EntitySelectionContext)
			{
				m_Gizmo.SetEntity(m_Panel->m_EntitySelectionContext);
				m_Gizmo.OnUpdate(ts);
				m_Gizmo.OnRender();
			}
			

			Renderer2D::EndScene();
		}
		
		
		
		


	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		std::string name = tag == std::string() ? "Entity" : tag;
		entity.AddComponent<TagComponent>(name);
		

		return entity;
	}
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		auto cameraComponents = m_Registry.view<CameraComponent>();
		for (entt::entity entt : cameraComponents)
		{
			CameraComponent& cam = cameraComponents.get<CameraComponent>(entt);
			if (!cam.fixedAspectRatio)
			{
				cam.camera.ResizeViewport(width, height);
				m_ViewportWidth = width;
				m_ViewPortHeight = height;
			}
		}
	}	

	void Scene::NewScene()
	{
		
		m_Panel->m_EntitySelectionContext = {};
		m_Registry.clear();
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	glm::vec2 Scene::GetSceneMousePos()
	{
		if (m_MainCamera)
		{

			SceneCamera::Bounds bounds = m_MainCamera->GetBounds();
			ImGuiIO& io = ImGui::GetIO();
			auto [x, y] = io.MousePos;
			x -= m_ViewportWindowPos.x;
			y = m_ViewPortHeight - y + m_ViewportWindowPos.y;
			x /= m_ViewportWidth / 2;
			y /= m_ViewPortHeight / 2;
			x -= 1;
			y -= 1;
			x *= bounds.right;
			y *= bounds.up;
			glm::vec4 mousePos{ x,y,0.0f,1.0f };
			mousePos = m_MainCameraTransform->GetTransform() * mousePos;

			return { mousePos.x,mousePos.y };
		}
		return { 0,0 };
	}

	template<typename T>
	void Scene::OnComponentAdded(T& component, Entity entity)
	{
		
	}

	template<>
	void Scene::OnComponentAdded<SpriteComponent>(SpriteComponent& component, Entity entity)
	{
		

	}
	
	template<>
	void Scene::OnComponentAdded<CameraComponent>(CameraComponent& component, Entity entity)
	{
		component.camera.ResizeViewport(m_ViewportWidth, m_ViewPortHeight);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(TagComponent& component, Entity entity)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(NativeScriptComponent& component, Entity entity)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(TransformComponent& component, Entity entity)
	{

	}

	std::ostream& operator <<(std::ostream& stream, const TagComponent& tag) { stream << tag.tag; return stream; }

}