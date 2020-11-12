#include "brpch.h"
#include "BrbeetorLayer.h"





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

		m_Square = m_ActiveScene->CreateEntity("Kocka");
	
		m_Square.AddComponent<SpriteComponent>(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

		auto& redSquare = m_ActiveScene->CreateEntity("Druga Kocka");
		redSquare.AddComponent<SpriteComponent>(glm::vec4(1.0f, 0.0f, 0.3f, 1.0f));
		
		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		auto& cam = m_CameraEntity.AddComponent<CameraComponent>();
		
		
		m_SecondCamera = m_ActiveScene->CreateEntity("Second camera");
		m_SecondCamera.AddComponent<CameraComponent>();
		m_SecondCamera.GetComponent<CameraComponent>().primary = false;

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

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		
	}

	void BrbeetorLayer::OnDetach()
	{
		BR_PROFILE_FUNCTION;
	}

	void BrbeetorLayer::OnUpdate(Brbanje::Timestep ts)
	{
		BR_PROFILE_FUNCTION;
		//update



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

				if (ImGui::MenuItem("Close"))
					Brbanje::Application::getApplication().Close();
				ImGui::EndMenu();
			}


			ImGui::EndMenuBar();
		}

		//Settings

		ImGui::Begin("Controller");
		ImGui::Text("Quad count: %d", Brbanje::Renderer2D::GetStats().QuadNumber);
		ImGui::Text("Draw calls: %d", Brbanje::Renderer2D::GetStats().DrawCalls);
		
		
		
		
		
		
		ImGui::End();

		//viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		
		
		ImGui::Begin("Viewport");
		
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
			
		ImGui::End();
		ImGui::PopStyleVar();

		m_SceneHierarchy.OnImGuiRender();

		ImGui::End();
		
	}

}
