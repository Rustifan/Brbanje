#include "brpch.h"
#include "Brbanje/Core/Log.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Brbanje/Core/Application.h"


// temp
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Brbanje
{
	
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
   
		BR_PROFILE_FUNCTION;
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
		
		io.Fonts->AddFontFromFileTTF("Assets/fonts/OpenSans-Bold.ttf", 18);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/fonts/OpenSans-Regular.ttf", 18);

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

		SetDarkThemeColor();
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::getApplication().getWindow().GetNativeWindow());

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::OnDetach()
	{
		BR_PROFILE_FUNCTION;
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		BR_PROFILE_FUNCTION;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		BR_PROFILE_FUNCTION;
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::getApplication();
		io.DisplaySize = ImVec2((float)app.getWindow().GetWidth(),(float)app.getWindow().GetHeight());

		//Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_contex = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_contex);

		}
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO io = ImGui::GetIO();
			//e.m_Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			//e.m_Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		

	}

	void ImGuiLayer::SetDarkThemeColor()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);

		//Headers
		colors[ImGuiCol_Header] = ImVec4(0.2f, 0.205f, 0.201, 1.0f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.305f, 0.301f, 1.0f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.115f, 0.151f, 1.0f);

		//Buttons
		colors[ImGuiCol_Button] = ImVec4(0.2f, 0.205f, 0.201, 1.0f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.305f, 0.301f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.115f, 0.151f, 1.0f);

		//Frame
		colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.205f, 0.201, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.305f, 0.301f, 1.0f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.115f, 0.151f, 1.0f);

		//Tabs
		colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.35f, 0.3505f, 0.351, 1.0f);
		colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.2505f, 0.251f, 1.0f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
		
		colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.95f, 0.1505f, 0.951f, 1.0f);



	}
}