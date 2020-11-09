#pragma once

#include "glm/glm.hpp"
#include "Brbanje/Renderer/Camera.h"
#include "Brbanje/Scene/SceneCamera.h"
#include "Brbanje/Scene/ScriptableEntity.h"

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
		std::function<void (ScriptableEntity*)> OnCreateFunction;
		std::function<void (ScriptableEntity*, Timestep)> OnUpdateFunction;
		std::function<void (ScriptableEntity*)> OnDestroyFunction;
		std::function<void ()> InstatiateFunction;
		std::function<void ()> DestroyInstanceFunction;
		
		template <typename T>
		void Bind()
		{
			InstatiateFunction = [&]() {instance = new T();};
			DestroyInstanceFunction = [&]() {delete (T*)instance;};
			OnCreateFunction = [](ScriptableEntity* instance) {((T*)instance)->OnCreate();};
			OnUpdateFunction = [](ScriptableEntity* instance, Timestep ts) {((T*)instance)->OnUpdate(ts);};
			OnDestroyFunction = [](ScriptableEntity* instance) {((T*)instance)->OnDestroy();};
		}

	};
}