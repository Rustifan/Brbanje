#pragma once

#include "entt.hpp"
#include "Brbanje/Core/Timestep.h"


namespace Brbanje
{
	class Entity;

	class Scene
	{
	private:
		entt::registry m_Registry;
	public:
		Scene();
		~Scene();
		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);
		Entity CreateEntity(const std::string& tag = std::string());
		
		
		friend class Entity;
	};
}