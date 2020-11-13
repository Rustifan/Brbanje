#include "brpch.h"
#include "Scene.h"
#include "Brbanje/Renderer/Renderer2D.h"
#include "Entity.h"
#include "Brbanje/Scene/ScriptableEntity.h"
#include "Components.h"


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

		
		Camera* mainCamera = nullptr;
		glm::mat4 transform;
		{
			auto group = m_Registry.group<>(entt::get<TransformComponent, CameraComponent>);
			for (auto entity : group)
			{
				auto [trasformComp, camera] = group.get<TransformComponent, CameraComponent>(entity);
				if (camera.primary)
				{
					mainCamera = &camera.camera;
					transform = trasformComp.GetTransform();
				}

			}
		}

		if (mainCamera)
		{

			Renderer2D::BeginScene(mainCamera->GetProjection(), transform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group)
			{
				if (m_Registry.has<SpriteComponent>(entity))
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

					Renderer2D::DrawQuad(transform.GetTransform(), sprite);
				}
				
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