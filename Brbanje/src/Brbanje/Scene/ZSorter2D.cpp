#include "brpch.h"
#include "ZSorter2D.h"
#include "Entity.h"
#include "Brbanje/Renderer/Renderer2D.h"

namespace Brbanje
{
	

	
	ZSorter2D::ZSorter2D(entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<SpriteComponent>, TransformComponent>* group):
		m_Group(group)
	{
	}

	void ZSorter2D::InsertEntity(float zCoord, Entity entity)
	{
		
		bool inserted = false;

		for (auto it = m_Map.begin(); it != m_Map.end(); ++it)
		{
			
			if (it->first == zCoord)
			{
				m_Map[zCoord].push_back(entity);
				inserted = true;
				break;
			}

			if (zCoord < it->first)
			{
				std::vector<Entity> vec = { entity };
				m_Map.insert(it, std::pair<float, std::vector<Entity>>(zCoord, vec));
			
				inserted = true;
				break;
			}
		}
		
		if (!inserted)
		{
			m_Map[zCoord].push_back(entity);
		}

		
	}

	void Brbanje::ZSorter2D::RenderEntitiesByZ(SceneCamera* camera, TransformComponent* cameraTransform)
	{

		for (auto& entities : m_Map)
		{
			std::vector<Entity>& vec = entities.second;
			Renderer2D::BeginScene(camera->GetProjection(), cameraTransform->GetTransform());

			for (auto& entity : vec)
			{
				SpriteComponent& sprite = m_Group->get<SpriteComponent>(entity);
				TransformComponent& transform = m_Group->get<TransformComponent>(entity);

				if (sprite.texture)
				{
					if (!sprite.subTexture)
					{
						Renderer2D::DrawQuad(transform.GetTransform(), sprite.texture, sprite.tilingFactor, sprite.color);

					}
					else
					{
						Renderer2D::DrawRotatedQuad(transform.position, transform.size, transform.rotation.z, sprite.subTexture);
					}
				}
				else
				{
					Renderer2D::DrawQuad(transform.GetTransform(), sprite);

				}

			}
			Renderer2D::EndScene();

		}


		
		
	}
}