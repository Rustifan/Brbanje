#pragma once
#include <string>
#include "glm/glm.hpp"

namespace Brbanje
{
	class ImGuiDrawFunctions
	{
		public:
			static void DrawVec2Control(const std::string& label, glm::vec2& values,float speed = 0.1f, float resetValue = 0.0f, float columnWidth = 100.0f, const char* format = "%.2f");
			


	};
}