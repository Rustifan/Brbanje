#include "brpch.h"
#include "SubTextureEditor.h"
#include "Scene.h"
#include "Entity.h"
#include "imgui.h"
#include "Brbanje/Renderer/Renderer2D.h"
#include "Components.h"
#include "Brbanje/Renderer/RenderCommand.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/ImGuiDrawFuctions.h"
#include "Brbanje/Core/Input.h"
#include "Brbanje/Core/KeyCodes.h"

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

	void SubTextureEditor::OnEvent(Event& event)
	{
		EventDispacher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(SubTextureEditor::OnKeyDown));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(SubTextureEditor::OnKeyUp));

	}

	bool SubTextureEditor::OnKeyDown(KeyPressedEvent& keyEvent)
	{
		if (keyEvent.GetKeyCode() == BR_KEY_LEFT_CONTROL)
		{
			m_Clip = true;
			
		}


		return false;
	}

	bool SubTextureEditor::OnKeyUp(KeyReleasedEvent& keyEvent)
	{
		if (keyEvent.GetKeyCode() == BR_KEY_LEFT_CONTROL)
		{
			m_Clip = false;
			
		}


		return false;
	}

	void SubTextureEditor::Edit()
	{
		if (m_Sprite)
		{
			if (m_Texture)
			{
				if (m_Texture->GetRendererID() != m_Sprite->texture->GetRendererID())
				{
					m_Texture = m_Sprite->texture;
					m_TextureSizeInPixel = { (float)m_Texture->GetWidth(), (float)m_Texture->GetHeight() };
					m_SubTexPos = { 0,0 };
					m_SubTexSize.x = m_Texture->GetWidth() / 10;
					m_SubTexSize.y = m_Texture->GetWidth() / 10;
				}
			}
			else
			{
				m_Texture = m_Sprite->texture;
				m_TextureSizeInPixel = { (float)m_Texture->GetWidth(), (float)m_Texture->GetHeight() };
				m_SubTexPos = { 0,0 };

				m_SubTexSize.x = m_Texture->GetWidth() / 10;
				m_SubTexSize.y = m_Texture->GetWidth() / 10;

			}
		}
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Texture Editor", &m_Edit);
		ImGui::Indent((ImGui::GetContentRegionAvailWidth() - m_TextureSize.x)/2);

		if (ImGui::GetWindowSize() != m_WindowSize)
		{
			m_WindowSize = ImGui::GetWindowSize();
			OnResize();
		}

		ImGui::Image((ImTextureID)m_Framebuffer->GetColorAttachmentId(), m_TextureSize, { 0,1 }, { 1,0 });
		
		if (IsMouseHovered(m_SubTexPos, m_SubTexSize) && ImGui::IsMouseClicked(0))
		{
			
			m_MoveSelection = true;
			m_MoveMouseDiff = glm::vec2(GetMousePos().x, GetMousePos().y) - m_SubTexPos;
		}
		if (ImGui::IsMouseReleased(0))
		{
			m_MoveSelection = false;
		}


		if (m_MoveSelection)
		{
			MoveSelection();
		}

		ImGui::End();
		ImGui::PopStyleVar();
		
		// Editor buttons

		ImGui::Begin("Texture Editor Controlls", &m_Edit);

		

		ImGuiDrawFunctions::DrawVec2Control("Sub Tex Position", m_SubTexPos,1.0f,0.0f,200.f,"%.0f");
		//clamp position
		m_SubTexPos.x = std::max(m_SubTexPos.x, 0.0f);
		m_SubTexPos.y = std::max(m_SubTexPos.y, 0.0f);
		if (m_SubTexPos.x + m_SubTexSize.x > m_TextureSizeInPixel.x) m_SubTexPos.x = m_TextureSizeInPixel.x - m_SubTexSize.x;
		if (m_SubTexPos.y + m_SubTexSize.y > m_TextureSizeInPixel.y) m_SubTexPos.y = m_TextureSizeInPixel.y - m_SubTexSize.y;

		ImGui::SameLine();

		ImGui::Checkbox("Clip (CTRL)", &m_Clip);
		
		
		ImGuiDrawFunctions::DrawVec2Control("Sub Tex Size", m_SubTexSize, 1.0f, std::max(m_TextureSizeInPixel.x, m_TextureSizeInPixel.y), 200.f, "%.0f");
		//clamp size
		if (m_SubTexSize.x > m_TextureSizeInPixel.x) m_SubTexSize.x = m_TextureSizeInPixel.x;
		if (m_SubTexSize.y > m_TextureSizeInPixel.y) m_SubTexSize.y = m_TextureSizeInPixel.y;
		m_SubTexSize.x = std::max(m_SubTexSize.x, 0.0f);
		m_SubTexSize.y = std::max(m_SubTexSize.y, 0.0f);
		

		ImGui::SameLine();

		ImGui::Text("Texture size: %d * %d", (int)m_TextureSizeInPixel.x, (int)m_TextureSizeInPixel.y);

		ImGui::Spacing();

		if (ImGui::Button("Set Sub texture"))
		{
			m_Sprite->subTexture = SubTexture2D::CreateFromPixCoords(m_Sprite->texture, m_SubTexPos,
				glm::vec2(m_SubTexPos.x + m_SubTexSize.x, m_SubTexPos.y + m_SubTexSize.y));
			
			m_Edit = false;
		}
		

		if (ImGui::Button("Use whole Texture"))
		{
			m_Sprite->subTexture = nullptr;

			m_Edit = false;
		}

		ImGui::SameLine();

		ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvailWidth() - 150.0f, 0.0f));

		ImGui::SameLine();

		if (ImGui::Button("Close"))
		{
			m_Edit = false;
		}
		
		
	
		
		
		ImGui::End();


	}

	void SubTextureEditor::MoveSelection()
	{
		glm::vec2& pos = m_SubTexPos;
		glm::vec2& size = m_SubTexSize;
		
		if (m_Clip)
		{
			int coef = GetMousePos().x  / size.x;
			pos.x = coef  * size.x;

			coef = GetMousePos().y / size.y;
			pos.y = coef  * size.y;
			
		}
		else
		{
			pos.x = GetMousePos().x - m_MoveMouseDiff.x;
			pos.y = GetMousePos().y - m_MoveMouseDiff.y;
		}
		


		//Clamp
		pos.x = std::max(pos.x, 0.0f);
		pos.y = std::max(pos.y, 0.0f);
		if (pos.x + m_SubTexSize.x > m_TextureSizeInPixel.x) pos.x = m_TextureSizeInPixel.x - m_SubTexSize.x;
		if (pos.y + m_SubTexSize.y > m_TextureSizeInPixel.y) pos.y = m_TextureSizeInPixel.y - m_SubTexSize.y;


	}

	ImVec2 SubTextureEditor::GetMousePos()
	{
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 TexPos = ImGui::GetItemRectMin();
		ImVec2 TexSize = ImGui::GetItemRectSize();
		mousePos.x -= TexPos.x;
		mousePos.y -= TexPos.y;
		mousePos.y = TexSize.y - mousePos.y;
		//from 0 to  1
		mousePos.x /= TexSize.x;
		mousePos.y /= TexSize.y;
		//Transfer to texture Pixel coordinates
		mousePos.x *= m_TextureSizeInPixel.x;
		mousePos.y *= m_TextureSizeInPixel.y;


		

		return mousePos;
	}

	bool SubTextureEditor::IsMouseHovered(const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 mousePos = *((glm::vec2*)&GetMousePos());
		return mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
			mousePos.y >= position.y && mousePos.y <= position.y + size.y;
		
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

			RenderCommand::SetColor({ 0.0f,0.0f,0,1.0f });
			RenderCommand::Clear();

			Renderer2D::BeginScene(m_FrameCamera.GetProjection(), m_FrameCameraTransform.GetTransform());

			float TexAspect = (float)m_Sprite->texture->GetWidth() / (float)m_Sprite->texture->GetHeight();
			//Draw Texture
			Renderer2D::DrawQuad({ 0,0 }, { TexAspect ,1 }, m_Sprite->texture);

			//Draw sub texture picker quad
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
		m_TextureSizeInPixel = { (float)PixW, (float)PixH };

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
		
		

		if (m_Entity)
		{
			float textureWidth = (float)m_Sprite->texture->GetWidth();
			float textureHeight = (float)m_Sprite->texture->GetHeight();
			float TexAspect = textureWidth / textureHeight;
			m_ViewPortSize = ImGui::GetContentRegionAvail();
			
			//Texture size 
			m_TextureSIzeMultipl = textureWidth >= textureHeight ? m_ViewPortSize.x / TexAspect : m_ViewPortSize.y;
			m_TextureSize.x = TexAspect * m_TextureSIzeMultipl;
			m_TextureSize.y = m_TextureSIzeMultipl;
			
			if (m_TextureSize.x > m_ViewPortSize.x) 
			{
				m_TextureSize.x = m_ViewPortSize.x;
				m_TextureSize.y = m_TextureSize.x / TexAspect;
			}
			if (m_TextureSize.y > m_ViewPortSize.y)
			{
				m_TextureSize.y = m_ViewPortSize.y;
				m_TextureSize.x = m_TextureSize.y * TexAspect;
			}
			
			
			
			
			

			m_Framebuffer->Resize(m_TextureSize.x, m_TextureSize.y);
			m_FrameCamera.ResizeViewport(m_TextureSize.x, m_TextureSize.y);
		}
		

	}


}