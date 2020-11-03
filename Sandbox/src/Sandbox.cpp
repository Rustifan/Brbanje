
#include "Brbanje.h"
//Entry
#include "Brbanje/Core/EntryPoint.h"
//temp
#include "Platform/OpenGl/OpenGlShader.h"
#include "Brbanje/Renderer/Shader.h"

#include "Sandbox2D.h"

class ExampleLayer :public Brbanje::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_CameraController(1.7f, true)
	{
		//Openg gl test
		

		m_VertexArray = Brbanje::VertexArray::Create();
		m_VertexArray->Bind();


		float vertices[3 * (3 + 4)] =
		{
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0,
			0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0,
			0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0


		};

		Brbanje::BufferLayout layout(

			{
				{Brbanje::ShaderDataType::Float3, "Posititon"},
				{Brbanje::ShaderDataType::Float4, "Color"}

			}
		);



		Brbanje::Ref<Brbanje::VertexBuffer> vertexBuffer;


		vertexBuffer = Brbanje::VertexBuffer::Create(vertices, sizeof(vertices));
		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);





		unsigned int indices[3] =
		{
			0,1,2,

		};

		Brbanje::Ref<Brbanje::IndexBuffer> indexBuffer;
		indexBuffer = Brbanje::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int));

		m_VertexArray->AddIndexBuffer(indexBuffer);


		m_Shader=Brbanje::Shader::Create("Assets/Shaders/FirstShader.glsl");


		m_SquareVA = Brbanje::VertexArray::Create();
		m_SquareVA->Bind();


		float SquareVertices[4 * (3 + 2)] =
		{							// U  V
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,	  1.0f, 0.0f,
			-0.5f, 0.5f, 0.0f,	  0.0f, 1.0f,
			0.5f, 0.5f, 0.0f,	  1.0f, 1.0f


		};

		Brbanje::Ref<Brbanje::VertexBuffer> squareVertex;
		squareVertex = Brbanje::VertexBuffer::Create(SquareVertices, sizeof(SquareVertices));
		squareVertex->SetLayout({
			{Brbanje::ShaderDataType::Float3, "Posititon"},
			{Brbanje::ShaderDataType::Float2, "TextureCoords"}
			}


		);
		m_SquareVA->AddVertexBuffer(squareVertex);

		unsigned int squareIndices[6] =
		{
			0,1,2,
			2,3,1

		};

		Brbanje::Ref<Brbanje::IndexBuffer> squareIndexes;
		squareIndexes = Brbanje::IndexBuffer::Create(squareIndices, sizeof(squareIndices));
		m_SquareVA->AddIndexBuffer(squareIndexes);





		m_BlueShader=Brbanje::Shader::Create("Assets/Shaders/ColorShader.glsl");

		

		auto textureShader = m_ShaderLibrary.Load("Assets/Shaders/Texture.glsl");
		textureShader->Attach();
		std::dynamic_pointer_cast<Brbanje::OpenGlShader>(textureShader)->UploadUniformInt("u_Texture", 0);
		
		
		
		;

		m_Texture = Brbanje::Texture2D::Create("Assets/Textures/zeldusana.png");
		m_AlfaTexture = Brbanje::Texture2D::Create("Assets/Textures/goose.png");
	}

	void OnUpdate(Brbanje::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);
		
		
		//Render
		Brbanje::RenderCommand::SetColor(glm::vec4(0.1f, 0.1f, 0.1f, 0));
		Brbanje::RenderCommand::Clear();


		Brbanje::Renderer::BeginScene(m_CameraController.GetCamera());
		m_BlueShader->Attach();
		m_BlueShader->SetFloat3("u_Color", m_SquareColor);


		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f) * 0.2f);
		

		//tiles test
		for (int y = -10; y < 11; ++y)
		{
			for (int x = -10; x < 11; ++x)
			{
				glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(x*0.21f, y*0.21f, 0.0f));
				
				Brbanje::Renderer::Submit(m_BlueShader, m_SquareVA, position * scale);
			}
		}
		
		Brbanje::Renderer::Submit(m_Shader, m_VertexArray,glm::mat4(1.0f)*glm::scale(glm::mat4(1.0f),glm::vec3(m_TriangleScale.x,m_TriangleScale.y,1.0f)));

		m_Texture->Bind(0);
		

		Brbanje::Renderer::Submit(m_ShaderLibrary.Get("Texture"), m_SquareVA, glm::scale(glm::mat4(1.f), glm::vec3(1.0f, 1.0f, 1.0f) * 2.0f));
		
		m_AlfaTexture->Bind(0);
		
		Brbanje::Renderer::Submit(m_ShaderLibrary.Get("Texture"), m_SquareVA, glm::scale(glm::mat4(1.f), glm::vec3(1.0f, 1.0f, 1.0f) * 2.0f));
		Brbanje::Renderer::EndScene();



		


		
	}

	void OnEvent(Brbanje::Event& event) override
	{
		m_CameraController.OnEvent(event);

	}


	virtual void OnImGuiRender()override
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::DragFloat2("Triangle Size", glm::value_ptr(m_TriangleScale),0.01f);

		ImGui::End();



	}

private:
	Brbanje::ShaderLibrary m_ShaderLibrary;
	Brbanje::Ref <Brbanje::VertexArray> m_VertexArray;
	Brbanje::Ref <Brbanje::Shader> m_Shader;

	Brbanje::Ref <Brbanje::VertexArray> m_SquareVA;
	Brbanje::Ref <Brbanje::Shader> m_BlueShader;
	
	Brbanje::Ref <Brbanje::Texture2D> m_Texture, m_AlfaTexture;

	Brbanje::OrtographicCameraController m_CameraController;

	

	glm::mat4 m_Transform = glm::mat4(1.0f);
	glm::vec3 m_SquareColor = glm::vec3(0.8f, 0.3f, 0.2f);
	glm::vec2 m_TriangleScale = glm::vec2(1.0f, 1.0f);
};




class Sandbox : public Brbanje::Application
{
public:
	Sandbox():Application()
	{
		//PushLayer(new ExampleLayer);
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}

	
	
};


Brbanje::Application* Brbanje::CreateApplication()
{
	return new Sandbox();
}