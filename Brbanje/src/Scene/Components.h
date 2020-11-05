#pragma once

#include "glm/glm.hpp"

namespace Brbanje
{
	struct TransformComponent
	{
		glm::mat4 transform;
		TransformComponent() = default;
		TransformComponent(const glm::mat4 transform) :
			transform(transform) {}
		TransformComponent(const TransformComponent& other) = default;
		operator glm::mat4& () { return transform; }
		operator const glm::mat4& ()const { return transform; }
	};

	struct SpriteComponent
	{
		glm::vec4 color;
		SpriteComponent() = default;
		SpriteComponent(const glm::vec4& color) :
			color(color) {}
		SpriteComponent(const SpriteComponent& other) = default;
		operator glm::vec4& () { return color; }
		operator const glm::vec4& ()const { return color; }
	};
}