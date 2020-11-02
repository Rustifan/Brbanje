#include "brpch.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Buffer.h"

#include "RenderCommand.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Brbanje
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoords;
		float texIndex;
		float tilingFactor;
	};
	
	struct Data2D
	{
		const uint32_t MAX_QUADS = 10000;
		const uint32_t MAX_QUAD_VERTICES = MAX_QUADS * 4;
		const uint32_t MAX_QUAD_INDICES = MAX_QUADS * 6;
		static const uint32_t MAX_TEXTURE_SLOTS = 32; //TODO DRIVER CAP

		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<VertexArray> SquareVA;
		std::array<Ref<Texture>, MAX_TEXTURE_SLOTS> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = whiteTexture

		uint32_t QuadIndexCount = 0;
		glm::vec4 QuadVerexPositions[4];
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPointer = nullptr;
		Renderer2D::Statistics Stats;
	};

	static Data2D s_Data;


	
	void Renderer2D::Init()
	{
		BR_PROFILE_FUNCTION;

		

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MAX_QUAD_VERTICES];

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whitePixel = 0xFFFFFFFF;
		s_Data.WhiteTexture->SetData(&whitePixel, sizeof(whitePixel));


		s_Data.TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		

		s_Data.SquareVA = VertexArray::Create();
		s_Data.SquareVA->Bind();


		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MAX_QUAD_VERTICES * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout(
			{
				{ShaderDataType::Float3, "Position"},
				{ShaderDataType::Float4, "Color"},
				{ShaderDataType::Float2, "TexCoords"},
				{ShaderDataType::Float, "TextureIndex"},
				{ShaderDataType::Float, "TilingFactor"}
			}
		);
		s_Data.SquareVA->AddVertexBuffer(s_Data.QuadVertexBuffer);

		uint32_t* quadIndices = new uint32_t[s_Data.MAX_QUAD_INDICES];

		uint32_t offset = 0;

		for (uint32_t i = 0; i < s_Data.MAX_QUAD_INDICES; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		s_Data.QuadVerexPositions[0] = { -0.5f,-0.5f,0.0f,1.0f };
		s_Data.QuadVerexPositions[1] = { 0.5f,-0.5f,0.0f,1.0f };
		s_Data.QuadVerexPositions[2] = { 0.5f, 0.5f,0.0f,1.0f };
		s_Data.QuadVerexPositions[3] = { -0.5f, 0.5f,0.0f,1.0f };


		Ref<IndexBuffer> squareIndex = IndexBuffer::Create(quadIndices, s_Data.MAX_QUAD_INDICES);
		s_Data.SquareVA->AddIndexBuffer(squareIndex);
		delete[] quadIndices;

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.TextureShader->Attach();
		int32_t samplers[s_Data.MAX_TEXTURE_SLOTS];
		for (uint32_t i = 0; i < s_Data.MAX_TEXTURE_SLOTS; i++)
			samplers[i] = i;
		s_Data.TextureShader->SetIntArray("sampler", samplers, s_Data.MAX_TEXTURE_SLOTS);

		
		
	}

	void Renderer2D::Shutdown()
	{
		BR_PROFILE_FUNCTION;
		
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		
		BR_PROFILE_FUNCTION;
		s_Data.TextureShader->Attach();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetProjectionViewMatrix());
		s_Data.QuadVertexBufferPointer = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;
		s_Data.TextureSlotIndex = 1;

	}	

	void Renderer2D::EndScene()
	{
		BR_PROFILE_FUNCTION;

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPointer - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.SquareVA, s_Data.QuadIndexCount);

		++s_Data.Stats.DrawCalls;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();
		
		s_Data.QuadVertexBufferPointer = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color )
	{
		BR_PROFILE_FUNCTION;
		
		if (s_Data.QuadIndexCount >= s_Data.MAX_QUAD_INDICES)
		{
			FlushAndReset();
		}
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f,0.0f,1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[0];
		s_Data.QuadVertexBufferPointer->color = color;
		s_Data.QuadVertexBufferPointer->texCoords = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPointer->texIndex = 0;
		s_Data.QuadVertexBufferPointer->tilingFactor = 1.0f;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[1];
		s_Data.QuadVertexBufferPointer->color = color;
		s_Data.QuadVertexBufferPointer->texCoords = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPointer->texIndex = 0;
		s_Data.QuadVertexBufferPointer->tilingFactor = 1.0f;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[2];
		s_Data.QuadVertexBufferPointer->color = color;
		s_Data.QuadVertexBufferPointer->texCoords = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPointer->texIndex = 0;
		s_Data.QuadVertexBufferPointer->tilingFactor = 1.0f;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[3];
		s_Data.QuadVertexBufferPointer->color = color;
		s_Data.QuadVertexBufferPointer->texCoords = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPointer->texIndex = 0;
		s_Data.QuadVertexBufferPointer->tilingFactor = 1.0f;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadIndexCount += 6;

		++s_Data.Stats.QuadNumber;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<Texture2D> texture, float tiling, glm::vec4 tintingColor)
	{
		
		DrawRotatedQuad(glm::vec3(position.x, position.y, 0.0f), size, rotation, texture, tiling);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<Texture2D> texture, float tiling, glm::vec4 tintingColor)
	{
		BR_PROFILE_FUNCTION;

		if (s_Data.QuadIndexCount >= s_Data.MAX_QUAD_INDICES)
		{
			FlushAndReset();
		}
		
		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
				textureIndex = (float)i; 
			break;
		}




		if (textureIndex == 0)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}


		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f,0.0f,1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[0];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[1];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[2];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[3];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadIndexCount += 6;

		++s_Data.Stats.QuadNumber;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color )
	{
		
		DrawRotatedQuad(glm::vec3(position.x, position.y, 0.0f), size, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		BR_PROFILE_FUNCTION;

		if (s_Data.QuadIndexCount >= s_Data.MAX_QUAD_INDICES)
		{
			FlushAndReset();
		}
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[0];
		s_Data.QuadVertexBufferPointer->color = color;
		s_Data.QuadVertexBufferPointer->texCoords = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPointer->texIndex = 0.0f;
		s_Data.QuadVertexBufferPointer->tilingFactor = 1.0f;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[1];
		s_Data.QuadVertexBufferPointer->color = color;
		s_Data.QuadVertexBufferPointer->texCoords = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPointer->texIndex = 0.0f;
		s_Data.QuadVertexBufferPointer->tilingFactor = 1.0f;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[2];
		s_Data.QuadVertexBufferPointer->color = color;
		s_Data.QuadVertexBufferPointer->texCoords = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPointer->texIndex = 0.0f;
		s_Data.QuadVertexBufferPointer->tilingFactor = 1.0f;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[3];
		s_Data.QuadVertexBufferPointer->color = color;
		s_Data.QuadVertexBufferPointer->texCoords = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPointer->texIndex = 0.0f;
		s_Data.QuadVertexBufferPointer->tilingFactor = 1.0f;

		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadIndexCount += 6;

		
		++s_Data.Stats.QuadNumber;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{

		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture, float tiling, glm::vec4 tintingColor)
	{
		BR_PROFILE_FUNCTION;
		
		if (s_Data.QuadIndexCount >= s_Data.MAX_QUAD_INDICES)
		{
			FlushAndReset();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
				textureIndex = (float)i; break;
		}
		
		
			

		if (textureIndex == 0)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}
		

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[0];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[1];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[2];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[3];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadIndexCount += 6;
		
		
		
		++s_Data.Stats.QuadNumber;
		
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D> texture, float tiling, glm::vec4 tintingColor)
	{

		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, texture, tiling, tintingColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<SubTexture2D> subTexture, float tiling, glm::vec4 tintingColor)
	{
		BR_PROFILE_FUNCTION;

		if (s_Data.QuadIndexCount >= s_Data.MAX_QUAD_INDICES)
		{
			FlushAndReset();
		}

		float textureIndex = 0.0f;
		Ref<Texture2D> texture = subTexture->GetTexture();


		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
				textureIndex = (float)i; break;
		}




		if (textureIndex == 0)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}


		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[0];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = subTexture->GetTetureCoords()[0];
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[1];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = subTexture->GetTetureCoords()[1];
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[2];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = subTexture->GetTetureCoords()[2];
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[3];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = subTexture->GetTetureCoords()[3];
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadIndexCount += 6;



		++s_Data.Stats.QuadNumber;

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<SubTexture2D> subTexture, float tiling, glm::vec4 tintingColor)
	{

		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, subTexture, tiling, tintingColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<SubTexture2D> subTexture, float tiling, glm::vec4 tintingColor)
	{
		BR_PROFILE_FUNCTION;

		if (s_Data.QuadIndexCount >= s_Data.MAX_QUAD_INDICES)
		{
			FlushAndReset();
		}

		Ref<Texture2D> texture = subTexture->GetTexture();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
				textureIndex = (float)i;
			break;
		}




		if (textureIndex == 0)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}


		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[0];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = subTexture->GetTetureCoords()[0];
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[1];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = subTexture->GetTetureCoords()[1];
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[2];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = subTexture->GetTetureCoords()[2];
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadVertexBufferPointer->position = transform * s_Data.QuadVerexPositions[3];
		s_Data.QuadVertexBufferPointer->color = tintingColor;
		s_Data.QuadVertexBufferPointer->texCoords = subTexture->GetTetureCoords()[3];
		s_Data.QuadVertexBufferPointer->texIndex = textureIndex;
		s_Data.QuadVertexBufferPointer->tilingFactor = tiling;
		s_Data.QuadVertexBufferPointer++;

		s_Data.QuadIndexCount += 6;

		++s_Data.Stats.QuadNumber;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<SubTexture2D> subTexture, float tiling, glm::vec4 tintingColor)
	{

		DrawRotatedQuad(glm::vec3(position.x, position.y, 0.0f), size, rotation, subTexture, tiling);
	}

	const Renderer2D::Statistics& Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(s_Data.Stats));
	}


}