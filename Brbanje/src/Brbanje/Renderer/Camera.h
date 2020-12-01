#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Brbanje
{
	class Camera
	{
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
		
	public:
		Camera(const glm::mat4& projection)
			:m_Projection(projection) {}

		Camera() = default;
		virtual ~Camera() = default;
		const glm::mat4& GetProjection() const { return m_Projection; }
		void SetProjection(const glm::mat4& projection) { m_Projection = projection; }
	
		
	};
}