#pragma once

#include "Brbanje/Core/Input.h"
#include "Brbanje/Core/Application.h"

namespace Brbanje
{
	class WindowsInput : public Input
	{
	public:
		virtual bool IsKeyPressedImpl(int KeyCode)const override;
		virtual bool IsMouseButtonPressedImpl(int button)const override;
		virtual float GetMouseXImpl()const override;
		virtual float GetMouseYImpl()const override;
		virtual std::pair<float, float> GetMousePosImpl()const override;
	};
}