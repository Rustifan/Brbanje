#pragma once

#include "Brbanje.h"

namespace Brbanje
{
	class BrbeetorLayer : public Layer
	{
	private:
		OrtographicCameraController m_CameraController;
		glm::vec4 m_SquareColor = glm::vec4(0.8f, 0.3f, 0.2f, 1.0f);
		Ref<Texture2D> m_Texture;
		Ref<Brbanje::Texture2D> m_Texture1;
		Brbanje::Ref<Brbanje::Texture2D> m_SpriteSheet;
		Brbanje::Ref<Brbanje::SubTexture2D> m_SubTex;
		Brbanje::Ref<Brbanje::Framebuffer> m_Framebuffer;
		std::unordered_map<char, Ref<SubTexture2D>> m_TileMap;

		
		struct ProfileResult
		{
			const char* name;
			float time;
		};
		std::vector<ProfileResult> m_ProfileResults;

	public:
		BrbeetorLayer();
		~BrbeetorLayer() = default;
		void OnAttach()	override;
		void OnDetach() override;
		void OnUpdate(Brbanje::Timestep ts)override;
		void OnEvent(Brbanje::Event& event)override;
		void OnImGuiRender()override;

	};

}