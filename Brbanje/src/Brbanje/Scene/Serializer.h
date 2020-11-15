#pragma once

#include "yaml-cpp/yaml.h"
#include "Scene.h"
#include "Entity.h"

namespace Brbanje
{
	class Serializer
	{
	private:
		Ref<Scene> m_Scene;
	private:
		void SerializeEntity(Entity entity, YAML::Emitter& out);
		

	public:
		Serializer(Ref<Scene> scene);
		void Serialize(const std::string& filePath);
		void Deserialize(const std::string& filePath);
		void SerializeBinary(const std::string& filePath);
		void DeserializeBinary(const std::string& filePath);
		void SerializeBinaryToBuffer(std::stringstream* buffer);
		
	

	};
	

}