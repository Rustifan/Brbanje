#include "brpch.h"
#include "Scene.h"
#include "Brbanje/Renderer/Renderer2D.h"

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
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite);
		}


	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}
}