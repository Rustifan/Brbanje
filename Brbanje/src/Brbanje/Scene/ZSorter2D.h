#pragma once

#include "Components.h"
#include "entt.hpp"
#include "SceneCamera.h"

namespace Brbanje
{
	class Entity;

	class ZSorter2D
	{
	private:
		entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<SpriteComponent>, TransformComponent>* m_Group;
		std::map<float, std::vector<Entity> > m_Map;
	public:
		ZSorter2D() = default;
		ZSorter2D(entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<SpriteComponent>, TransformComponent>* group);
		void SetGroup(entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<SpriteComponent>, TransformComponent>* group) { m_Group = group; }
		void InsertEntity(float zCoord, Entity entity);
		void RenderEntitiesByZ(SceneCamera* camera, TransformComponent* cameraTransform);
	};

}