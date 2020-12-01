#include "brpch.h"
#include "SubTextureEditor.h"
#include "Scene.h"
#include "Entity.h"
#include "imgui.h"
#include "Brbanje/Renderer/Renderer2D.h"
#include "Components.h"
#include "Brbanje/Renderer/RenderCommand.h"
#include "glm/gtc/type_ptr.hpp"

namespace Brbanje
{
	SubTextureEditor::SubTextureEditor(Scene* scene) : m_Scene(scene)
	{
		FramebufferSpecifications frameSpecs;
		frameSpecs.width = 800;
		frameSpecs.height = 600;
		m_Framebuffer = Framebuffer::Create(frameSpecs);
		m_FrameCamera = SceneCamera();
		m_FrameCamera.SetSize(1);

		m_FrameCameraTransform = TransformComponent();
	}

	void SubTextureEditor::OnImGuiRender()
	{
		if (m_Edit)
		{
			Edit();
		}
	}

	static bool operator!=(const ImVec2& first, const ImVec2& second)
	{
		return first.x != second.x || first.y != second.y;
	}

	void SubTextureEditor::Edit()
	{
		ImGui::Begin("Texture Editor", &m_Edit);
		ImGui::Indent((ImGui::GetContentRegionAvailWidth() - m_TextureSize.x)/2);

		ImGui::BeginChild("Texture", m_TextureSize, false);
		ImGui::Image((ImTextureID)m_Framebuffer->GetColorAttachmentId(), m_TextureSize, { 0,1 }, { 1,0 });

		ImGui::EndChild();
		
		
		
		ImGui::DragFloat2("Sub Tex Pos", glm::value_ptr(m_SubTexPos), 1, m_Sprite->texture->GetWidth());
		ImGui::DragFloat2("Sub Tex Size", (float*)&m_SubTexSize, 1, m_Sprite->texture->GetWidth());

		if (ImGui::Button("Set "))
		{
			m_Sprite->subTexture = SubTexture2D::CreateFromPixCoords(m_Sprite->texture, m_SubTexPos,
				glm::vec2(m_SubTexPos.x + m_SubTexSize.x, m_SubTexPos.y + m_SubTexSize.y));
			
			m_Edit = false;
		}
		
		
		
		
		if (ImGui::GetWindowSize() != m_WindowSize)
		{
			m_WindowSize = ImGui::GetWindowSize();
			OnResize();
		}
	
		
		
		ImGui::End();


	}

	
	void SubTextureEditor::Update(Timestep ts)
	{
		if (m_Entity != m_Scene->m_Panel->m_EntitySelectionContext)
		{
			m_Edit = false;
			if (!m_Scene->m_Panel->m_EntitySelectionContext)
			{
				m_Entity = {};
				m_Sprite = nullptr;
			}
			else if(m_Scene->m_Panel->m_EntitySelectionContext.HasComponent<SpriteComponent>())
			{
				m_Entity = m_Scene->m_Panel->m_EntitySelectionContext;
				m_Sprite = &(m_Entity.GetComponent<SpriteComponent>());
				if (m_Sprite->texture)
				{
					float TexAspect = (float)m_Sprite->texture->GetWidth() / (float)m_Sprite->texture->GetHeight();
					m_TextureSize.x = TexAspect * m_TextureSIzeMultipl;
					m_TextureSize.y = m_TextureSIzeMultipl;
					m_Framebuffer->Resize(m_TextureSize.x, m_TextureSize.y);
					m_FrameCamera.ResizeViewport(m_TextureSize.x, m_TextureSize.y);
				}
			}

			//To DO Clear gizmo pos
		}

		if (m_Edit)
		{
			m_Framebuffer->Bind();

			RenderCommand::SetColor({ 0.0f,0.2f,0,1.0f });
			RenderCommand::Clear();

			Renderer2D::BeginScene(m_FrameCamera.GetProjection(), m_FrameCameraTransform.GetTransform());

			float TexAspect = (float)m_Sprite->texture->GetWidth() / (float)m_Sprite->texture->GetHeight();
			Renderer2D::DrawQuad({ 0,0 }, { TexAspect ,1 }, m_Sprite->texture);

			BR_TRACE(m_SubTexPos.x);
			glm::vec2 pos = FromPixelToWorld(m_SubTexPos);
			
			glm::vec2 size = FromPixelToWorld(m_SubTexSize, true);

			Renderer2D::DrawQuad({ pos.x+size.x/2, pos.y+size.y/2, 0.01f }, { size.x, size.y }, { 0,1,0,0.5f });
			


			Renderer2D::EndScene();
			m_Framebuffer->Unbind();

		}
		
	}

	glm::vec2 SubTextureEditor::FromPixelToWorld(const glm::vec2& value, bool size)
	{
		float TexAspect = (float)m_Sprite->texture->GetWidth() / (float)m_Sprite->texture->GetHeight();

		uint32_t PixW = m_Sprite->texture->GetWidth();
		uint32_t PixH = m_Sprite->texture->GetHeight();

		float width = TexAspect;
		float height = 1;
		//

		float xPos = value.x, yPos = value.y;
		xPos /= PixW;
		yPos /= PixH;
		xPos *= width;
		yPos *= height;
		if (!size)
		{
			xPos -= width / 2;
			yPos -= height / 2;
		}
		

		return glm::vec2(xPos, yPos);
	}

	void SubTextureEditor::OnResize()
	{
		m_TextureSIzeMultipl = m_WindowSize.x/2;

		if (m_Entity)
		{
			float TexAspect = (float)m_Sprite->texture->GetWidth() / (float)m_Sprite->texture->GetHeight();
			m_TextureSize.x = TexAspect * m_TextureSIzeMultipl;
			m_TextureSize.y = m_TextureSIzeMultipl;
			m_Framebuffer->Resize(m_TextureSize.x, m_TextureSize.y);
			m_FrameCamera.ResizeViewport(m_TextureSize.x, m_TextureSize.y);
		}
		

	}


}