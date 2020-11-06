#include "brpch.h"
#include "Scene.h"
#include "Brbanje/Renderer/Renderer2D.h"
#include "Entity.h"

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
		
		
		Camera* mainCamera = nullptr;
		glm::mat4* transform = nullptr;
		{
			auto group = m_Registry.group<>(entt::get<TransformComponent, CameraComponent>);
			for (auto entity : group)
			{
				auto& [trasformComp, camera] = group.get<TransformComponent, CameraComponent>(entity);
				if (camera.primary)
				{
					mainCamera = &camera.camera;
					transform = &trasformComp.transform;
				}

			}
		}

		if (mainCamera)
		{

			Renderer2D::BeginScene(mainCamera->GetProjection(), *transform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group)
			{
				if (m_Registry.has<SpriteComponent>(entity))
				{
					auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

					Renderer2D::DrawQuad(transform, sprite);
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

	std::ostream& operator <<(std::ostream& stream, const TagComponent& tag) { stream << tag.tag; return stream; }

}