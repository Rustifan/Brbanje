#include "brpch.h"
#include "Shader.h"
#include "Platform/OpenGl/OpenGlShader.h"
#include "Renderer.h"


namespace Brbanje
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc)
	{

				switch (Renderer::GetAPI())
				{
				case RendererAPI::API::None:
					BR_CORE_ASSERT(false, "No Renderer api selected")

						break;
				case RendererAPI::API::OpenGl:
					return std::make_shared<OpenGlShader>(name, VertexShaderSrc, FragmentShaderSrc);
				}
				return nullptr;

		
	}

	Ref<Shader> Shader::Create(const std::string& fileSrc)
	{

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BR_CORE_ASSERT(false, "No Renderer api selected")

				break;
		case RendererAPI::API::OpenGl:
			return std::make_shared<OpenGlShader>(fileSrc);
		}
		return nullptr;


	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		
		
		BR_CORE_ASSERT((m_ShaderMap.find(name) == m_ShaderMap.end()), "shader with that name already exist")
		
		m_ShaderMap[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		BR_CORE_ASSERT((m_ShaderMap.find(name) == m_ShaderMap.end()), "shader with that name already exist")

		m_ShaderMap[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		Ref<Shader> shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		Ref<Shader> shader = Shader::Create(filePath);
		Add(name,shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		BR_CORE_ASSERT((m_ShaderMap.find(name) != m_ShaderMap.end()), "There is no shader with that name")
		return m_ShaderMap[name];
	}
}