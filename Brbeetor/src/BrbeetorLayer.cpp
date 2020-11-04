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



	
	}

	void BrbeetorLayer::OnDetach()
	{
		BR_PROFILE_FUNCTION;
	}

	void BrbeetorLayer::OnUpdate(Brbanje::Timestep ts)
	{
		BR_PROFILE_FUNCTION;
		//update


		m_CameraController.OnUpdate(ts);

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

			Brbanje::Renderer2D::BeginScene(m_CameraController.GetCamera());

			static float rotation = 0.0f;
			rotation++;
			if (rotation > 360) rotation = 0.0f;
			Brbanje::Renderer2D::DrawRotatedQuad({ 0.5f,-0.5f,-0.1 }, glm::vec2(1.0f, 1.0f), glm::radians(rotation), m_Texture, 2, { 0.5,0.5,1,1 });


			for (int i = 0; i < 10; i++)
			{
				Brbanje::Renderer2D::DrawQuad({ i,i + 1,0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,0.0f,1.0f });
				Brbanje::Renderer2D::DrawRotatedQuad({ i,i, }, glm::vec2(1.0f, 1.0f), glm::radians(92.0f), m_Texture, 2, { 0.5,0.5,1,1 });
			}

			Brbanje::Renderer2D::DrawRotatedQuad({ 1.0f,0.0f }, { 0.5f,2.0f }, glm::radians(rotation), { 0.0f,0.5f,0.1f,1.0f });
			Brbanje::Renderer2D::DrawQuad({ 1.0f,0.0f }, { 0.5f,2.0f }, { m_SquareColor });

			Brbanje::Renderer2D::DrawQuad({ -1.0f,0.1f,0.0f }, { 1.0f,1.0f }, m_Texture);
			Brbanje::Renderer2D::DrawQuad({ 0.0f,-1.1f }, { 0.5f,0.5f }, m_Texture);
			Brbanje::Renderer2D::DrawQuad({ -1.0, -1.0 }, { 0.5f, 0.5f }, m_Texture1, 2.0f, { 1.0f,0.2f,0.2f,1.0f });

			for (uint32_t y = 0; y < 10; ++y)
			{
				for (uint32_t x = 0; x < 10; ++x)
				{
					Brbanje::Renderer2D::DrawQuad({ float(x) * 0.2f, float(y) * 0.2f }, { 0.19f,0.19f }, { 0.2f,0.3f,0.7f,1.0f });
				}
			}

			

			Brbanje::Renderer2D::EndScene();




			Brbanje::Renderer2D::BeginScene(m_CameraController.GetCamera());


		





			Brbanje::Renderer2D::EndScene();
			m_Framebuffer->Unbind();
		}




	}

	void BrbeetorLayer::OnEvent(Brbanje::Event& event)
	{
		BR_PROFILE_FUNCTION;
		m_CameraController.OnEvent(event);
	}

	void BrbeetorLayer::OnImGuiRender()
	{
		BR_PROFILE_FUNCTION;

		static bool dockingEnabled = true;


		if (dockingEnabled)
		{
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
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
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
			
			ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
			
			if (*((glm::vec2*)&viewPortSize)!=m_ViewportSize)
			{
				m_ViewportSize.x = viewPortSize.x;
				m_ViewportSize.y = viewPortSize.y;
				m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
				m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			}
			ImGui::Image((void*)m_Framebuffer->GetColorAttachmentId(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, { 0,1 }, { 1,0 });
			
			ImGui::End();
			ImGui::PopStyleVar();

			ImGui::End();
		}
	}

}
