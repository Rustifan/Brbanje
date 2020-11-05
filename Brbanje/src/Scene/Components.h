#pragma once

#include "glm/glm.hpp"

namespace Brbanje
{
	struct TagComponent
	{
		std::string tag;
		TagComponent() = default;
		TagComponent(const std::string& tag) :
			tag(tag) {}
		TagComponent(const TagComponent& other) = default;
		operator std::string&() { return tag; }
		operator const std::string& ()const { return tag; }
		
	};
	std::ostream& operator <<(std::ostream& stream, const TagComponent& tag);
	
	struct TransformComponent
	{
		glm::mat4 transform = glm::mat4(1.0f);
		TransformComponent() = default;
		TransformComponent(const glm::mat4 transform) :
			transform(transform) {}
		TransformComponent(const TransformComponent& other) = default;
		operator glm::mat4& () { return transform; }
		operator const glm::mat4& ()const { return transform; }
	};

	struct SpriteComponent
	{
		glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
		SpriteComponent() = default;
		SpriteComponent(const glm::vec4& color) :
			color(color) {}
		SpriteComponent(const SpriteComponent& other) = default;
		operator glm::vec4& () { return color; }
		operator const glm::vec4& ()const { return color; }
	};
}