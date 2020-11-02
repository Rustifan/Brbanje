#include "Sandbox2D.h"
#include <memory>
#include "Platform/OpenGl/OpenGlShader.h"
#include "Brbanje/Renderer/Shader.h"

Sandbox2D::Sandbox2D(): Layer("Sandbox2D"), m_CameraController(1280.0f/720.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	
	BR_PROFILE_FUNCTION;
	
	m_Texture = Brbanje::Texture2D::Create("Assets/Textures/zeldusana.png");
	m_Texture1 = Brbanje::Texture2D::Create("Assets/Textures/beer.png");
	
}

void Sandbox2D::OnDetach()
{
	BR_PROFILE_FUNCTION;
}

void Sandbox2D::OnUpdate(Brbanje::Timestep ts)
{	
	BR_PROFILE_FUNCTION;
	//update
	
		
	m_CameraController.OnUpdate(ts);
	
	//reset stats

	Brbanje::Renderer2D::ResetStats();

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
		Brbanje::Renderer2D::DrawRotatedQuad({ 0.5f,-0.5f,-0.1 }, glm::vec2(1.0f, 1.0f), rotation, m_Texture, 2, {0.5,0.5,1,1});
		

		for (int i = 0; i < 10; i++)
		{
			Brbanje::Renderer2D::DrawQuad({ i,i+1,0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,0.0f,1.0f });
			Brbanje::Renderer2D::DrawRotatedQuad({ i,i,}, glm::vec2(1.0f, 1.0f), 95.0f, m_Texture, 2, { 0.5,0.5,1,1 });
		}

		Brbanje::Renderer2D::DrawRotatedQuad({ 1.0f,0.0f }, { 0.5f,2.0f }, rotation, { 0.0f,0.5f,0.1f,1.0f });
		Brbanje::Renderer2D::DrawQuad({ 1.0f,0.0f }, { 0.5f,2.0f }, { m_SquareColor });
		
		Brbanje::Renderer2D::DrawQuad({ -1.0f,0.1f,0.0f }, { 1.0f,1.0f }, m_Texture);
		Brbanje::Renderer2D::DrawQuad({ 0.0f,-1.1f }, { 0.5f,0.5f }, m_Texture);
		Brbanje::Renderer2D::DrawQuad({ -1.0, -1.0 }, { 0.5f, 0.5f }, m_Texture1, 2.0f, {1.0f,0.2f,0.2f,1.0f});

		for (uint32_t y = 0; y < 10; ++y)
		{
			for (uint32_t x = 0; x < 10; ++x)
			{
				Brbanje::Renderer2D::DrawQuad({ float(x) * 0.2f, float(y) * 0.2f }, { 0.19f,0.19f }, { 0.2f,0.3f,0.7f,1.0f });
			}
		}

		Brbanje::Renderer2D::EndScene();
	}
	
}

void Sandbox2D::OnEvent(Brbanje::Event& event)
{
	BR_PROFILE_FUNCTION;
	m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	BR_PROFILE_FUNCTION;
	ImGui::Begin("Controller");

	
	ImGui::Text("Quad count: %d", Brbanje::Renderer2D::GetStats().QuadNumber);
	ImGui::Text("Draw calls: %d", Brbanje::Renderer2D::GetStats().DrawCalls);
	

	ImGui::ColorPicker4("Square Color", (float*)&m_SquareColor);
	

	ImGui::End();
}


