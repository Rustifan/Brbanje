#pragma once

#include <string>
#include "glm/glm.hpp"

namespace Brbanje
{
	class Shader
	{
	public:

		virtual ~Shader() = default;
		virtual void Attach() = 0;
		virtual void Detach() = 0;
		virtual const std::string &GetName() const = 0;
		static Ref<Shader> Create(const std::string& name, const std::string &VertexShaderSrc, const std::string& FragmentShaderSrc);
		static Ref<Shader> Create(const std::string& fileSrc);
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& vector) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& vector) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* value, uint32_t count) = 0;
	};


	class ShaderLibrary
	{
	private:
		std::unordered_map<std::string, Ref<Shader>> m_ShaderMap;
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string &filePath);
		Ref<Shader> Load(const std::string &name,const std::string &filePath);
		Ref<Shader> Get(const std::string& name);
		


	};
}