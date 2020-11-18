#include "brpch.h"
#include "Brbanje/Utils/FileDialog.h"
#include "commdlg.h"
#include "winuser.h"
#include "Brbanje/Core/Application.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace Brbanje
{
	std::optional<std::string> FileDialog::OpenFile(const char* filter)
	{
		GLFWwindow* window = (GLFWwindow*)Application::getApplication().getWindow().GetNativeWindow();
		HWND nativeWindow = glfwGetWin32Window(window);
		char filepath[200];
		OPENFILENAMEA ofn;
		ZeroMemory(&filepath, sizeof(filepath));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nativeWindow;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = filepath;
		ofn.nMaxFile = 200;
		ofn.lpstrTitle = "Open File";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn))
		{
			return std::string(filepath);
		}
		
		return {};
	

		
	}

	std::optional<std::string> FileDialog::SaveFile(const char*filter)
	{
		GLFWwindow* window = (GLFWwindow*)Application::getApplication().getWindow().GetNativeWindow();
		HWND nativeWindow = glfwGetWin32Window(window);
		char filepath[200];
		OPENFILENAMEA ofn;
		ZeroMemory(&filepath, sizeof(filepath));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nativeWindow;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = filepath;
		ofn.nMaxFile = 200;
		ofn.lpstrTitle = "Save Scene";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn))
		{
			return std::string(filepath);
		}

		return {};
	}

	bool FileDialog::YesNoWindow(const char* caption, const char* message)
	{
		GLFWwindow* window = (GLFWwindow*)Application::getApplication().getWindow().GetNativeWindow();
		HWND nativeWindow = glfwGetWin32Window(window);
		const uint32_t maxLenght = 1000;
		
		
		wchar_t wCaption[maxLenght];
		wchar_t wMessage[maxLenght];
		std::mbstowcs(wCaption, caption, strlen(caption)+1);
		std::mbstowcs(wMessage, message, strlen(message)+1);
		
		
		//6 = YES  7 = NO
		if (MessageBox(nativeWindow, wMessage, wCaption, MB_YESNO | MB_ICONQUESTION) == 6)
		{
			return true;
		}
		return false;

	}
}