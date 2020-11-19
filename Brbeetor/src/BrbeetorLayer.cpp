#include "brpch.h"
#include "BrbeetorLayer.h"
#include "Brbanje/Scene/Serializer.h"
#include "Editor/SceneHierarchyPanel.h"
#include "Brbanje/Utils/FileDialog.h"
#include <optional>


namespace Brbanje
{

	BrbeetorLayer::BrbeetorLayer() : Layer("BrbeetorLayer"), m_CameraController(1280.0f / 720.0f, true)
	{
		
	}

	void BrbeetorLayer::OnAttach()
	{

		BR_PROFILE_FUNCTION;

		m_Texture = Brbanje::Texture2D::Create("Assets/Textures/zeldusana.png");
		m_Texture1 = Brbanje::Texture2D::Create("Assets/Textures/beer.png");
		m_SpriteSheet = Brbanje::Texture2D::Create("Assets/game/Spritesheet/spriteSheet.png");

		Brbanje::FramebufferSpecifications fbSpec;
		fbSpec.width = 1280; fbSpec.height = 720;
		m_Framebuffer = Brbanje::Framebuffer::Create(fbSpec);
		
		m_ActiveScene = std::make_shared<Scene>();

		m_SceneHierarchy.SetContext(m_ActiveScene);
	
		

		

		class CameraController: public ScriptableEntity
		{
		private:
			TransformComponent* transform;
			glm::vec2 direction{ 0.0f,0.0f};
			float moveSpeed = 5.0f;
		public:
			void OnCreate()
			{
				transform = &GetComponent<TransformComponent>();
				
			
			}

			void OnUpdate(Timestep ts)
			{
				transform->position.x += direction.x * moveSpeed * (float)ts;
				transform->position.y += direction.y * moveSpeed * (float)ts;
				
				if (Input::IsKeyPressed(BR_KEY_A))
					direction.x = -1.0f;
				
				
				else if (Input::IsKeyPressed(BR_KEY_D))
					direction.x = 1.0f;
				else direction.x = 0.0f;
				
				if (Input::IsKeyPressed(BR_KEY_S))
					direction.y = -1.0f;
				
				
				else if (Input::IsKeyPressed(BR_KEY_W))
					direction.y = 1.0f;
				else direction.y = 0.0f;
				
				if (direction.x != 0 || direction.y != 0)
					direction = glm::normalize(direction);

				
			}

			void OnDestroy()
			{

			}

		};

		
		
	}

	void BrbeetorLayer::OnDetach()
	{
		BR_PROFILE_FUNCTION;
	}

	void BrbeetorLayer::OnUpdate(Brbanje::Timestep ts)
	{
		BR_PROFILE_FUNCTION;
		//update

		OnKeyPress();

		if (m_IsViewportFocused)
		{
			m_CameraController.OnUpdate(ts);
		}
		//reset stats

		

		Brbanje::Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		//render
		{
			BR_PROFILE_SCOPE("Renderer Prep");
			Brbanje::RenderCommand::SetColor(glm::vec4(0.1f, 0.1f, 0.1f, 0));
			Brbanje::RenderCommand::Clear();
		}

		{
			BR_PROFILE_SCOPE("Renderer Draw");

			

				
			m_ActiveScene->OnUpdate(ts);

			




			m_Framebuffer->Unbind();
		}

		class CameraController : public ScriptableEntity
		{
			
		};



	}

	void BrbeetorLayer::OnEvent(Brbanje::Event& event)
	{
		BR_PROFILE_FUNCTION;
		if (m_IsViewportFocused)
		{
			m_CameraController.OnEvent(event);
		}
		
	}

	void BrbeetorLayer::OnImGuiRender()
	{
		BR_PROFILE_FUNCTION;

		ImGuiIO& io = ImGui::GetIO();

		

		
		static bool isOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &isOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSize;

		
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{

				if (ImGui::MenuItem("New (CTRL + N)"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Open (CTRL + O)"))
				{
					LoadScene();

				}

				if (ImGui::MenuItem("Save (CTRL + S)"))
				{
					SaveScene();
				}

				if (ImGui::MenuItem("Save As (CTRL + SHIFT + S)"))
				{
					SaveAsScene();
				}

				

				if (ImGui::MenuItem("Close (CTRL + SHIFT + Q)"))
				{
					Quit();
				}


				ImGui::EndMenu();

			}


			ImGui::EndMenuBar();
		}

		//Settings

		ImGui::Begin("Controller");
		ImGui::Text("Quad count: %d", Renderer2D::GetStats().QuadNumber);
		ImGui::Text("Draw calls: %d", Renderer2D::GetStats().DrawCalls);
		ImGui::Text("Vertex count: %d", Renderer2D::GetStats().VertexCount());
		ImGui::Text("Index count: %d", Renderer2D::GetStats().IndexCount());
		
		
		
		
		
		ImGui::End();

	

		//viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		
		
		ImGui::Begin("Viewport");
		

		
		m_ActiveScene->SetMouseHoveredOnVieport(ImGui::IsWindowHovered());
		

		
		m_IsViewportFocused = ImGui::IsWindowFocused();
		m_IsViewportHovered = ImGui::IsWindowHovered();

		


		Application::getApplication().GetImGuiLayer()->SetBlockEvents(!m_IsViewportFocused || !m_IsViewportHovered);
		
		ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
			
		if (*((glm::vec2*)&viewPortSize)!=m_ViewportSize)
		{
			m_ViewportSize.x = viewPortSize.x;
			m_ViewportSize.y = viewPortSize.y;
			m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			
		}

		

		ImGui::Image((void*)m_Framebuffer->GetColorAttachmentId(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, { 0,1 }, { 1,0 });
		
		m_MainWindowPos = ImGui::GetItemRectMin();
		m_ActiveScene->SetViewportWindowPos(*((glm::vec2*)& m_MainWindowPos));

		ImGui::End();
		ImGui::PopStyleVar();

		m_SceneHierarchy.OnImGuiRender();

		ImGui::End();
		
	}

	void BrbeetorLayer::LoadScene()
	{
		
		Serializer serializer(m_ActiveScene);
		std::stringstream ss;
		serializer.SerializeBinaryToBuffer(&ss);
		if (ss.str().compare(m_ActiveScene->GetSaveBuffer().str()) != 0)
		{
			if (FileDialog::YesNoWindow("Save scene?", "You changed scene since last save. Do you want to save scene"))
			{
				SaveScene();
			}
		}
		

		std::optional<std::string> loadPath = FileDialog::OpenFile("Brbanje file(*.brb)\0*brb\0");
		if (loadPath)
		{
			serializer.Deserialize(loadPath.value());
			m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_SavePath = loadPath.value();
		}
		
	}

	void BrbeetorLayer::SaveAsScene()
	{
		Serializer serializer(m_ActiveScene);
		std::optional<std::string> filePath = FileDialog::SaveFile("Brbanje file(*.brb)\0*.brb\0");
		if (filePath)
		{
			std::string ext = filePath.value().substr(filePath.value().length() - 4, 4);
			if (ext.compare(".brb") == 0)
			{
				serializer.Serialize(filePath.value());
				m_SavePath = filePath.value();
			}
			else
			{
				serializer.Serialize(filePath.value() + ".brb");
				m_SavePath = filePath.value() + ".brb";
			}
			m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			
		}
		
	}

	void BrbeetorLayer::SaveScene()
	{
		if (m_SavePath.compare("") == 0)
		{
			SaveAsScene();
		}
		else
		{
			Serializer serializer(m_ActiveScene);
			serializer.Serialize(m_SavePath);
		}
	}

	void BrbeetorLayer::NewScene()
	{
		Serializer serializer(m_ActiveScene);
		std::stringstream ss;
		serializer.SerializeBinaryToBuffer(&ss);
		if (ss.str().compare(m_ActiveScene->GetSaveBuffer().str()) != 0)
		{
			if (FileDialog::YesNoWindow("Save scene?", "You changed scene since last save. Do you want to save scene?"))
			{
				SaveScene();
			}
		}
		
		
		
		m_ActiveScene->NewScene();
		m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		m_SavePath = "";
		m_ActiveScene->ClearSaveBuffer();
		
		

	}

	void BrbeetorLayer::Quit()
	{
		Serializer serializer(m_ActiveScene);
		std::stringstream ss;
		serializer.SerializeBinaryToBuffer(&ss);
		if (ss.str().compare(m_ActiveScene->GetSaveBuffer().str()) != 0)
		{
			if (FileDialog::YesNoWindow("Save scene?", "You changed scene since last save. Do you want to save scene?"))
			{
				SaveScene();
			}
		}

		if (FileDialog::YesNoWindow("Quit?", "Do you really want to quit?"))
		{
			Application::getApplication().Close();
		}
	}

	void BrbeetorLayer::OnKeyPress()
	{
		bool ctrlPress = Input::IsKeyPressed(BR_KEY_LEFT_CONTROL) || Input::IsKeyPressed(BR_KEY_RIGHT_CONTROL);
		bool shiftPress = Input::IsKeyPressed(BR_KEY_LEFT_SHIFT) || Input::IsKeyPressed(BR_KEY_RIGHT_SHIFT);

		//New Scene
		if (ctrlPress && Input::IsKeyPressed(BR_KEY_N))
		{
			NewScene();
		}

		if (ctrlPress && shiftPress && Input::IsKeyPressed(BR_KEY_S))
		{
			SaveAsScene();
		}

		if (ctrlPress && Input::IsKeyPressed(BR_KEY_S))
		{
			SaveScene();
		}

		if (ctrlPress && Input::IsKeyPressed(BR_KEY_O))
		{
			LoadScene();
		}

		if (ctrlPress && shiftPress && Input::IsKeyPressed(BR_KEY_Q))
		{
			Quit();
		}

		
		
	
		
	}

}
