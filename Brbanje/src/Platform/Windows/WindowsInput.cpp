#include "brpch.h"
#include "WindowsInput.h"
#include "GLFW/glfw3.h"

namespace Brbanje
{
	Input* Input::s_Instance = new WindowsInput;

	bool WindowsInput::IsKeyPressedImpl(int KeyCode)const
	{
		
		GLFWwindow* NativeWindow =(GLFWwindow*)Application::getApplication().getWindow().GetNativeWindow();
		return glfwGetKey(NativeWindow, KeyCode)==GLFW_PRESS || glfwGetKey(NativeWindow, KeyCode) == GLFW_REPEAT ? true : false;

	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)const
	{
		GLFWwindow* NativeWindow = static_cast<GLFWwindow*>(Application::getApplication().getWindow().GetNativeWindow());
	
		return glfwGetMouseButton(NativeWindow, button) == GLFW_PRESS;

	}

	float WindowsInput::GetMouseXImpl()const
	{
		GLFWwindow* NativeWindow = static_cast<GLFWwindow*>(Application::getApplication().getWindow().GetNativeWindow());
		double x;
		glfwGetCursorPos(NativeWindow, &x, nullptr);
		return (float)x;
	}

	float WindowsInput::GetMouseYImpl()const
	{
		GLFWwindow* NativeWindow = static_cast<GLFWwindow*>(Application::getApplication().getWindow().GetNativeWindow());
		double y;
		glfwGetCursorPos(NativeWindow, nullptr, &y);
		return (float)y;
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl()const
	{
		GLFWwindow* NativeWindow = static_cast<GLFWwindow*>(Application::getApplication().getWindow().GetNativeWindow());
		double x, y;
		
		glfwGetCursorPos(NativeWindow, &x, &y);
		return std::pair<float, float>((float)x,(float) y);
	}


}