#pragma once

#include "Core.h"

namespace Brbanje
{
	class Brbanje_API Input
	{
	public:
		static bool IsKeyPressed(int KeyCode);
		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePos();
	

	};


}