#pragma once

#include "Brbanje/Renderer/Texture.h"
#include "Brbanje/Renderer/SubTexture2D.h"
#include "Brbanje/Renderer/Framebuffer.h"
#include "Brbanje/Scene/SceneCamera.h"
#include "Components.h"
#include "imgui.h"

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
		float m_TextureSIzeMultipl = 500.0f;
		ImVec2 m_TextureSize;
		ImVec2 m_WindowSize = { 0,0 };
		ImVec2 m_ViewPortSize = {0,0};
		glm::vec2 m_SubTexSize = {0 ,0 };
		glm::vec2 m_SubTexPos = { 0,0 };
	private:
		void OnResize();
		glm::vec2 FromPixelToWorld(const glm::vec2& value, bool size = false);
	public:
	;
		SubTextureEditor(Scene* scene);
		void OnImGuiRender();
		void Edit();
		void Update(Timestep ts);
		void SetEdit(bool edit) { m_Edit = edit; }

	};

}