#pragma once

#include "entt.hpp"
#include "Brbanje/Core/Timestep.h"
#include "Components.h"

namespace Brbanje
{
	class Scene
	{
	private:
		entt::registry m_Registry;
	public:
		Scene();
		~Scene();
		void OnUpdate(Timestep ts);
		entt::entity CreateEntity();
		entt::registry& Reg() { return m_Registry; }
	};
}