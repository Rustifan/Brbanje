#pragma once

#include <string>
#include "glm/glm.hpp"
#include "Brbanje/Renderer/Shader.h"

namespace Brbanje
{
	class OpenGlShader: public Shader
	{
	private:
		std::string m_Name;
		uint32_t program = 0;
	private:
		void CreateShader(const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc);
	public:
		OpenGlShader(const std::string& name, const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc);
		OpenGlShader(const std::string& fileSrc);
		virtual ~OpenGlShader();
		virtual void Attach()override;
		virtual void Detach()override;
		virtual const std::string& GetName()const override { return m_Name; }
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& vector) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& vector) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* value, uint32_t count)override;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformVec4(const std::string& name, const glm::vec4& vec);
		void UploadUniformVec3(const std::string& name, const glm::vec3& vec);
		void UploadUniformVec2(const std::string& name, const glm::vec2& vec);
		void UploadUniformFloat(const std::string& name, const float& value);
		void UploadUniformInt(const std::string& name, const int& value);
		void UploadUniformIntArray(const std::string& name, int* value, uint32_t count);
	};
}