#pragma once

#include "Core.h"

namespace Brbanje
{
	class Brbanje_API Input
	{
	public:
		static bool IsKeyPressed(int KeyCode) { return s_Instance->IsKeyPressedImpl(KeyCode); };
		static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
	protected:
		static Input* s_Instance;
		virtual bool IsKeyPressedImpl(int KeyCode)const = 0;
		virtual bool IsMouseButtonPressedImpl(int button)const = 0;
		virtual std::pair<float, float> GetMousePosImpl()const = 0;
		virtual float GetMouseXImpl()const = 0;
		virtual float GetMouseYImpl()const = 0;


	};


}