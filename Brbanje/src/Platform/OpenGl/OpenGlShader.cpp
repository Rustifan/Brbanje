#include "brpch.h"
#include "OpenGlShader.h"
#include <fstream>
#include "glad/glad.h"
#include "Brbanje/Core/Log.h"
#include "glm/gtc/type_ptr.hpp"

namespace Brbanje
{
	OpenGlShader::OpenGlShader(const std::string& name, const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc):
		m_Name(name)
	{
		BR_PROFILE_FUNCTION;
		CreateShader(VertexShaderSrc, FragmentShaderSrc);
	}

	OpenGlShader::OpenGlShader(const std::string& fileSrc)
	{
		BR_PROFILE_FUNCTION;

		auto lastBackLine = fileSrc.find_last_of("/\\");
		lastBackLine = lastBackLine == std::string::npos ? 0 : lastBackLine + 1;

		auto dotPos = fileSrc.rfind(".");
		auto count = dotPos == std::string::npos ? fileSrc.size() : dotPos - lastBackLine;
		
		m_Name = fileSrc.substr(lastBackLine, count);
		
		enum class ShaderType
		{
			NONE = 0, VERTEX, FRAGMENT
		}shaderType = ShaderType::NONE;
		int lineNumber=0;
		std::stringstream vertexShader, fragmentShader;
		std::ifstream file;
		file.open(fileSrc);
		BR_CORE_ASSERT(file, "could not open shader file")

			while (file)
			{
				lineNumber++;
				std::string line;
				std::getline(file, line);
				if (line.substr(0, 6) == "#type ")
				{
					std::string type = line.substr(6);
					if (type == "vertex")
					{
						shaderType = ShaderType::VERTEX;
					}
					else if (type == "fragment")
					{
						shaderType = ShaderType::FRAGMENT;
					}
					
					else
					{
						BR_CORE_ERROR("invalid type of shader at line {0}", lineNumber);
						BR_CORE_ASSERT(false, "invalid type of shader")
					}
					
				}
				else
				{
					switch (shaderType)
					{
					case ShaderType::NONE:
						break;
					case ShaderType::VERTEX:
						vertexShader << line << "\n";
						break;
					case ShaderType::FRAGMENT: 
						fragmentShader << line << "\n";
						break;
					}
				}
				
			}
		
		file.close();

		
		
		std::string vert = vertexShader.str();
		std::string frag = fragmentShader.str();
		
		
		BR_CORE_ASSERT(!vert.empty(), "there is no vertex shader")
		BR_CORE_ASSERT(!frag.empty(), "there is no fragment shader")

		CreateShader(vert, frag);
		
		


		


	}

	void OpenGlShader::CreateShader(const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc)
	{
		BR_PROFILE_FUNCTION;
		// Read our shaders into the appropriate buffers
		const std::string& vertexSource = VertexShaderSrc;
		const std::string& fragmentSource = FragmentShaderSrc;

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			BR_CORE_ERROR("Vertex Shader compilation error: {0}", &infoLog[0]);
			// In this simple program, we'll just leave
			BR_CORE_ASSERT(false, "Vertex Shader compilation error");
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			BR_CORE_ERROR("Fragment Shader compilation error: {0}", &infoLog[0]);
			// In this simple program, we'll just leave
			BR_CORE_ASSERT(false, "Fragment Shader compilation error");
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		program = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			BR_CORE_ERROR("Shader linking error: {0}", &infoLog[0]);
			// In this simple program, we'll just leave
			BR_CORE_ASSERT(false, "Shader linking error");
		}

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	OpenGlShader::~OpenGlShader()
	{
		BR_PROFILE_FUNCTION;
		glDeleteProgram(program);
	}

	void OpenGlShader::Attach()
	{
		BR_PROFILE_FUNCTION;
		glUseProgram(program);
	}

	void OpenGlShader::Detach()
	{
		BR_PROFILE_FUNCTION;
		glUseProgram(0);
	}

	void OpenGlShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		BR_PROFILE_FUNCTION;
		UploadUniformMat4(name, matrix);
	}

	void OpenGlShader::SetFloat3(const std::string& name, const glm::vec3& vector)
	{
		BR_PROFILE_FUNCTION;
		UploadUniformVec3(name, vector);
	}

	void OpenGlShader::SetFloat4(const std::string& name, const glm::vec4& vector)
	{
		BR_PROFILE_FUNCTION;
		UploadUniformVec4(name, vector);
	}

	void OpenGlShader::SetFloat(const std::string& name, float value)
	{
		BR_PROFILE_FUNCTION;
		UploadUniformFloat(name, value);
	}

	void OpenGlShader::SetInt(const std::string& name, int value)
	{
		BR_PROFILE_FUNCTION;
		UploadUniformInt(name, value);	
	}

	void OpenGlShader::SetIntArray(const std::string& name, int* value, uint32_t count)
	{
		UploadUniformIntArray(name, value, count);
	}

	void OpenGlShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		int uniformLocation = glGetUniformLocation(program, name.c_str());
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGlShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		int uniformLocation = glGetUniformLocation(program, name.c_str());
		glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	}


	void OpenGlShader::UploadUniformVec4(const std::string & name, const glm::vec4 & vec)
	{
		int uniformLocation = glGetUniformLocation(program, name.c_str());
		glUniform4fv(uniformLocation, 1, glm::value_ptr(vec));
	}

	void OpenGlShader::UploadUniformVec3(const std::string& name, const glm::vec3& vec)
	{
		int uniformLocation = glGetUniformLocation(program, name.c_str());
		glUniform3fv(uniformLocation, 1, glm::value_ptr(vec));
	}

	void OpenGlShader::UploadUniformVec2(const std::string& name, const glm::vec2& vec)
	{
		int uniformLocation = glGetUniformLocation(program, name.c_str());
		glUniform3fv(uniformLocation, 1, glm::value_ptr(vec));
	}

	void OpenGlShader::UploadUniformFloat(const std::string& name, const float& value)
	{
		int uniformLocation = glGetUniformLocation(program, name.c_str());
		glUniform1f(uniformLocation,value);
	}

	void OpenGlShader::UploadUniformInt(const std::string& name, const int& value)
	{
		int uniformLocation = glGetUniformLocation(program, name.c_str());
		glUniform1i(uniformLocation, value);
	}

	void OpenGlShader::UploadUniformIntArray(const std::string& name, int* value, uint32_t count)
	{
		int uniformLocation = glGetUniformLocation(program, name.c_str());
		glUniform1iv(uniformLocation, count, value);
	}
}