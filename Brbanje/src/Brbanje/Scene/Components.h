#pragma once

#include "glm/glm.hpp"
#include "Brbanje/Renderer/Camera.h"
#include "Brbanje/Scene/SceneCamera.h"
#include "Brbanje/Scene/ScriptableEntity.h"
#include "Brbanje/Renderer/Texture.h"

namespace Brbanje
{
	struct TagComponent
	{
		std::string tag;
		uint32_t Entity_ID;
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
		glm::vec3 position = { 0.0f,0.0f,0.0f };
		glm::vec3 size = { 1.0f,1.0f,1.0f };
		glm::vec3 rotation = { 0.0f,0.0f,0.0f };


		TransformComponent() = default;
		TransformComponent(const glm::vec3 position) :
			position(position) {}
		TransformComponent(const TransformComponent& other) = default;
		
		glm::mat4 GetTransform()const
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *

				glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
				glm::scale(glm::mat4(1.0f), size);
			
			return transform;
		}
	};

	struct SpriteComponent
	{
		glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
		Ref<Texture2D> texture;
		float tilingFactor = 1.0f;

		SpriteComponent() = default;
		SpriteComponent(const glm::vec4& color) :
			color(color) {}
		SpriteComponent(const SpriteComponent& other) = default;
		operator glm::vec4& () { return color; }
		operator const glm::vec4& ()const { return color; }
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool primary = true;
		bool fixedAspectRatio = false;
		CameraComponent() = default;
		
		CameraComponent(const CameraComponent& other) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* instance;
		
		ScriptableEntity* (*InstatiateFunction)();
		void (*DestroyInstanceFunction)(NativeScriptComponent*);
		
		template <typename T>
		void Bind()
		{
			InstatiateFunction = []() {return (ScriptableEntity*)(new T());};
			DestroyInstanceFunction = [](NativeScriptComponent* instance) {delete (T*)instance;};
			
		}

	};
}