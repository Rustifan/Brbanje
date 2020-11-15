#include "brpch.h"
#include "Serializer.h"
#include <fstream>
#include "Components.h"

namespace Brbanje
{
	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& vec)
	{
		out << YAML::BeginSeq;
		out << vec.x << vec.y << vec.z;
		out << YAML::EndSeq;

		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec4& vec)
	{
		out << YAML::BeginSeq;
		out << vec.x << vec.y << vec.z << vec.a;
		out << YAML::EndSeq;

		return out;
	}


	
	
	Serializer::Serializer(Ref<Scene> scene) :m_Scene(scene)
	{

	}
	
	void Serializer::SerializeEntity(Entity entity, YAML::Emitter& out)
	{
		

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << entity.GetComponent<TagComponent>().tag;
		out << YAML::Key << "Components" << YAML::Value;
		out << YAML::BeginMap;

		if (entity.HasComponent<TransformComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			
			
			//transform
			out << YAML::Key << "transform component" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "position" << YAML::Value << transform.position;
			out << YAML::Key << "rotation" << YAML::Value <<  transform.rotation;
			out << YAML::Key << "size" << YAML::Value << transform.size;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<SpriteComponent>())
		{
			auto& sprite = entity.GetComponent<SpriteComponent>();

			//sprite
			out << YAML::Key << "sprite component" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "color" << YAML::Value << sprite.color;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<CameraComponent>())
		{
			auto& cc = entity.GetComponent<CameraComponent>();
			auto& camera = cc.camera;
			
			out << YAML::Key << "camera component" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "primary" << YAML::Value << cc.primary;
			out << YAML::Key << "projection" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "perspective FOV" << YAML::Value << camera.GetPerspectiveFOV();
			out << YAML::Key << "perspective near" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "perspective far" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "orthographic size" << YAML::Value << camera.GetSize();
			out << YAML::Key << "orthographic near" << YAML::Value << (float)camera.GetOrthographicNearClip();
			out << YAML::Key << "orthographic far" << YAML::Value << (float)camera.GetOrthographicFarClip();
			out << YAML::Key << "fixed aspect ratio" << YAML::Value << cc.fixedAspectRatio;
			out << YAML::EndMap;

		}



		out << YAML::EndMap;
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	void Serializer::Serialize(const std::string& filePath)
	{
		SerializeBinaryToBuffer(&m_Scene->m_saveBuffer);

		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "TheScene";
		out << YAML::Key << "Entities" << YAML::Key;
		out << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](entt::entity entityID) {

			Entity entity(entityID, m_Scene.get());
			SerializeEntity(entity, out);



			});
		out << YAML::EndSeq;

		out << YAML::EndMap;
		std::ofstream output;
		output.open(filePath);
		BR_CORE_ASSERT(output.is_open(), "Could not open path for serializing")
		output << out.c_str();
		output.close();
		
	}

	void Serializer::Deserialize(const std::string& filePath)
	{

		YAML::Node node = YAML::LoadFile(filePath);
		if (node["Entities"])
		{

			m_Scene->NewScene();

			YAML::Node entities = node["Entities"];
			for (auto& ent : entities)
			{
				auto entityNode = ent["Entity"];
				Entity entity = m_Scene->CreateEntity(entityNode["Name"].as<std::string>());

				auto componentsNode = entityNode["Components"];

				if (componentsNode["transform component"]) //Transform component
				{
					TransformComponent& transform = entity.GetComponent<TransformComponent>();
					auto transformNode = componentsNode["transform component"];
					BR_CORE_ASSERT(transformNode["position"], "Position missing")
					glm::vec3 position;
					position.x = transformNode["position"][0].as<float>();
					position.y = transformNode["position"][1].as<float>();
					position.z = transformNode["position"][2].as<float>();
					transform.position = position;

					BR_CORE_ASSERT(transformNode["rotation"], "Rotation missing")
					glm::vec3 rotation;
					rotation.x = transformNode["rotation"][0].as<float>();
					rotation.y = transformNode["rotation"][1].as<float>();
					rotation.z = transformNode["rotation"][2].as<float>();
					transform.rotation = rotation;

					BR_CORE_ASSERT(transformNode["size"], "Size missing")

				    glm::vec3 size;
					size.x = transformNode["size"][0].as<float>();
					size.y = transformNode["size"][1].as<float>();
					size.z = transformNode["size"][2].as<float>();
					transform.size = size;

				}

				if (componentsNode["sprite component"])
				{
					SpriteComponent& sprite = entity.AddComponent<SpriteComponent>();
					auto spriteNode = componentsNode["sprite component"];
					BR_CORE_ASSERT(spriteNode["color"], "Color missing")

					glm::vec4 color;
					color.r = spriteNode["color"][0].as<float>();
					color.g = spriteNode["color"][1].as<float>();
					color.b = spriteNode["color"][2].as<float>();
					color.a = spriteNode["color"][3].as<float>();
					sprite.color = color;

				}

				if (componentsNode["camera component"])
				{
					CameraComponent& camera = entity.AddComponent<CameraComponent>();
					auto cameraNode = componentsNode["camera component"];

					camera.primary = cameraNode["primary"].as<bool>();
					camera.camera.SetProjectionType ((SceneCamera::ProjectionType)cameraNode["projection"].as<int>());
					camera.camera.SetPerspectiveFOV(cameraNode["perspective FOV"].as<float>());
					camera.camera.SetOrthoGraphicNearClip(cameraNode["perspective near"].as<float>());
					camera.camera.SetOrthoGraphicFarClip(cameraNode["perspective far"].as<float>());
					camera.camera.SetSize(cameraNode["orthographic size"].as<float>());
					camera.camera.SetOrthoGraphicNearClip(cameraNode["orthographic near"].as<float>());
					camera.camera.SetOrthoGraphicFarClip(cameraNode["orthographic far"].as<float>());
					camera.fixedAspectRatio = cameraNode["fixed aspect ratio"].as<bool>();
					
				}
				
			}


		}
		SerializeBinaryToBuffer(&m_Scene->m_saveBuffer);

		

	}

#pragma pack(push, 1)
	struct EntityData
	{
		char name[100];
		//Transform
		bool hasTranform;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 size;
		//Sprite
		bool hasSprite;
		glm::vec4 color;
		//Camera
		bool hasCamera;
		bool isPrimary;
		bool isPerspective;
		float perspectiveFOV;
		float perspectiveNear;
		float perspectiveFar;
		float orthoSize;
		float orthoNear;
		float orthoFar;
		bool fixedAspect;
	};
#pragma pack(pop)

	void Serializer::SerializeBinary(const std::string& filePath)
	{
		std::ofstream stream;
		stream.open(filePath, std::ios::binary);
		BR_CORE_ASSERT(stream.is_open(), "Could not save binary file");
		uint32_t entityCount = m_Scene->m_Registry.alive();
		stream.write((char*)&entityCount, sizeof(entityCount));
		m_Scene->m_Registry.each([&](entt::entity entityID) {

			EntityData data;
			Entity entity(entityID, m_Scene.get());

			
			strcpy(data.name, entity.GetComponent<TagComponent>().tag.c_str());
			if (entity.HasComponent<TransformComponent>())
			{
				data.hasTranform = true;
				TransformComponent& transform = entity.GetComponent<TransformComponent>();
				data.position = transform.position;
				data.rotation = transform.rotation;
				data.size = transform.size;
			}
			else
			{
				data.hasTranform = false;
				data.position = { 0,0,0};
				data.rotation = {0,0,0};
				data.size = { 0,0,0 };
			}
			if (entity.HasComponent<SpriteComponent>())
			{
				data.hasSprite = true;
				SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
				data.color = sprite.color;
			}
			else
			{
				data.hasSprite = false;
				data.color = { 0,0,0,0 };
			}
			if (entity.HasComponent<CameraComponent>())
			{
				data.hasCamera = true;
				CameraComponent& cc = entity.GetComponent<CameraComponent>();
				SceneCamera& camera = cc.camera;
				data.isPrimary = cc.primary;
				data.isPerspective = camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective ? true : false;
				data.perspectiveFOV = camera.GetPerspectiveFOV();
				data.perspectiveNear = camera.GetPerspectiveNearClip();
				data.perspectiveFar = camera.GetPerspectiveFarClip();
				data.orthoSize = camera.GetSize();
				data.orthoNear = camera.GetOrthographicNearClip();
				data.orthoFar = camera.GetOrthographicFarClip();
			}
			else
			{
				data.hasCamera = false;
				data.isPrimary = false;
				data.isPerspective = false;
				data.perspectiveFOV = 0;
				data.perspectiveNear = 0;
				data.perspectiveFar = 0;
				data.orthoNear = 0;
				data.orthoFar = 0;
			}
			
			stream.write((char*)&data, sizeof(data));

			});
		
		
		
		stream.close();
	}

	void Serializer::DeserializeBinary(const std::string& filePath)
	{
		m_Scene->NewScene();

		std::ifstream stream;
		stream.open(filePath, std::ios::binary);
		BR_CORE_ASSERT(stream.is_open(), "Could not open binary file");
		uint32_t entityCount = 0;
		
		stream.read((char*)&entityCount, sizeof(uint32_t));

		
		for (uint32_t i = 0; i < entityCount; ++i)
		{
			EntityData data;
			stream.read((char*)&data, sizeof(EntityData));
			Entity entity = m_Scene->CreateEntity(data.name);
			if (data.hasTranform)
			{
				TransformComponent& transform = entity.GetComponent<TransformComponent>();
				transform.position = data.position;
				transform.rotation = data.rotation;
				transform.size = data.size;
			}
			else
			{
				entity.RemoveComponent<TransformComponent>();
			}
			if (data.hasSprite)
			{
				SpriteComponent& sprite = entity.AddComponent<SpriteComponent>();
				sprite.color = data.color;

			}
			if (data.hasCamera)
			{
				CameraComponent& cc = entity.AddComponent<CameraComponent>();
				SceneCamera& camera = cc.camera;
				cc.primary = data.isPrimary;
				camera.SetProjectionType(data.isPerspective ? SceneCamera::ProjectionType::Perspective : SceneCamera::ProjectionType::Orthographic);
				camera.SetPerspectiveFOV(data.perspectiveFOV);
				camera.SetPerspectiveNearClip(data.perspectiveNear);
				camera.SetPerspectiveFarClip(data.perspectiveFar);
				camera.SetSize(data.orthoSize);
				camera.SetOrthoGraphicNearClip(data.orthoNear);
				camera.SetOrthoGraphicFarClip(data.orthoFar);
				cc.fixedAspectRatio = data.fixedAspect;
			}
		}
		

		

		

		stream.close();
		
	}

	void Serializer::SerializeBinaryToBuffer(std::stringstream* buffer)
	{
		buffer->str(std::string());
		
		uint32_t entityCount = m_Scene->m_Registry.alive();
		if (entityCount == 0)return;
		buffer->write((char*)&entityCount, sizeof(entityCount));
		m_Scene->m_Registry.each([&](entt::entity entityID) {

			EntityData data;
			Entity entity(entityID, m_Scene.get());


			strcpy(data.name, entity.GetComponent<TagComponent>().tag.c_str());
			if (entity.HasComponent<TransformComponent>())
			{
				data.hasTranform = true;
				TransformComponent& transform = entity.GetComponent<TransformComponent>();
				data.position = transform.position;
				data.rotation = transform.rotation;
				data.size = transform.size;
			}
			else
			{
				data.hasTranform = false;
				data.position = { 0,0,0 };
				data.rotation = { 0,0,0 };
				data.size = { 0,0,0 };
			}
			if (entity.HasComponent<SpriteComponent>())
			{
				data.hasSprite = true;
				SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
				data.color = sprite.color;
			}
			else
			{
				data.hasSprite = false;
				data.color = { 0,0,0,0 };
			}
			if (entity.HasComponent<CameraComponent>())
			{
				data.hasCamera = true;
				CameraComponent& cc = entity.GetComponent<CameraComponent>();
				SceneCamera& camera = cc.camera;
				data.isPrimary = cc.primary;
				data.isPerspective = camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective ? true : false;
				data.perspectiveFOV = camera.GetPerspectiveFOV();
				data.perspectiveNear = camera.GetPerspectiveNearClip();
				data.perspectiveFar = camera.GetPerspectiveFarClip();
				data.orthoSize = camera.GetSize();
				data.orthoNear = camera.GetOrthographicNearClip();
				data.orthoFar = camera.GetOrthographicFarClip();
			}
			else
			{
				data.hasCamera = false;
				data.isPrimary = false;
				data.isPerspective = false;
				data.perspectiveFOV = 0;
				data.perspectiveNear = 0;
				data.perspectiveFar = 0;
				data.orthoNear = 0;
				data.orthoFar = 0;
			}

			buffer->write((char*)&data, sizeof(data));

			});
	}
	



}