#pragma once

#include "entt.hpp"
#include "Brbanje/Core/Timestep.h"
#include "Components.h"

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
		Entity CreateEntity(const std::string& tag = std::string());
		

		friend class Entity;
	};
}