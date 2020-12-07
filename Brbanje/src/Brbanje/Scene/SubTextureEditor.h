#pragma once

#include "Brbanje/Renderer/Texture.h"
#include "Brbanje/Renderer/SubTexture2D.h"
#include "Brbanje/Renderer/Framebuffer.h"
#include "Brbanje/Scene/SceneCamera.h"
#include "Components.h"
#include "imgui.h"
#include "Brbanje/Events/Event.h"
#include "Brbanje/Events/KeyEvent.h"

namespace Brbanje
{
	class Scene;
	

	class SubTextureEditor
	{
	private:
		Scene* m_Scene;
		
		Entity m_Entity;
		Ref<Framebuffer> m_Framebuffer;
		Ref<Texture2D> m_Texture;
		Ref<SubTexture2D> m_SubTexture;
		SceneCamera m_FrameCamera;
		TransformComponent m_FrameCameraTransform;
		SpriteComponent* m_Sprite = nullptr;
		bool m_Edit = false;
		bool m_MoveSelection = false;
		bool m_Clip = false;
		glm::vec2 m_MoveMouseDiff;
		float m_TextureSIzeMultipl = 500.0f;
		ImVec2 m_TextureSize;
		ImVec2 m_WindowSize = { 0,0 };
		ImVec2 m_ViewPortSize = {0,0};
		glm::vec2 m_SubTexSize = {128.0f ,128.0f };
		glm::vec2 m_SubTexPos = { 0,0 };
		glm::vec2 m_TextureSizeInPixel;
		
	private:
		ImVec2 GetMousePos();
		void OnResize();
		glm::vec2 FromPixelToWorld(const glm::vec2& value, bool size = false);
		bool IsMouseHovered(const glm::vec2& position, const glm::vec2& size);
		void MoveSelection();
		bool OnKeyDown(KeyPressedEvent& keyEvent);
		bool OnKeyUp(KeyReleasedEvent& keyEvent);

	public:
	
		SubTextureEditor(Scene* scene);
		void OnImGuiRender();
		void Edit();
		void Update(Timestep ts);
		void SetEdit(bool edit) { m_Edit = edit; }
		void OnEvent(Event& event);
		bool IsEditing() { return m_Edit; }
	};

}