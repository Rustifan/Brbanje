#include "brpch.h"
#include "Brbanje/Core/Input.h"
#include "Brbanje/Core/Application.h"
#include "GLFW/glfw3.h"

namespace Brbanje
{
	

	bool Input::IsKeyPressed(int KeyCode)
	{
		
		GLFWwindow* NativeWindow =(GLFWwindow*)Application::getApplication().getWindow().GetNativeWindow();
		return glfwGetKey(NativeWindow, KeyCode)==GLFW_PRESS || glfwGetKey(NativeWindow, KeyCode) == GLFW_REPEAT ? true : false;

	}

	bool Input::IsMouseButtonPressed(int button)
	{
		GLFWwindow* NativeWindow = static_cast<GLFWwindow*>(Application::getApplication().getWindow().GetNativeWindow());
	
		return glfwGetMouseButton(NativeWindow, button) == GLFW_PRESS;

	}

	float Input::GetMouseX()
	{
		GLFWwindow* NativeWindow = static_cast<GLFWwindow*>(Application::getApplication().getWindow().GetNativeWindow());
		double x;
		glfwGetCursorPos(NativeWindow, &x, nullptr);
		return (float)x;
	}

	float Input::GetMouseY()
	{
		GLFWwindow* NativeWindow = static_cast<GLFWwindow*>(Application::getApplication().getWindow().GetNativeWindow());
		double y;
		glfwGetCursorPos(NativeWindow, nullptr, &y);
		return (float)y;
	}

	std::pair<float, float> Input::GetMousePos()
	{
		GLFWwindow* NativeWindow = static_cast<GLFWwindow*>(Application::getApplication().getWindow().GetNativeWindow());
		double x, y;
		
		glfwGetCursorPos(NativeWindow, &x, &y);
		return std::pair<float, float>((float)x,(float) y);
	}


}