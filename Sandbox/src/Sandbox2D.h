#pragma once

#include "Brbanje.h"
#include "ParticleSystem.h"

class Sandbox2D : public Brbanje::Layer
{
private:
	

	Brbanje::OrtographicCameraController m_CameraController;
	glm::vec4 m_SquareColor = glm::vec4(0.8f, 0.3f, 0.2f, 1.0f);
	Brbanje::Ref<Brbanje::Texture2D> m_Texture;
	Brbanje::Ref<Brbanje::Texture2D> m_Texture1;
	Brbanje::Ref<Brbanje::Texture2D> m_SpriteSheet;
	Brbanje::Ref<Brbanje::SubTexture2D> m_SubTex;
	std::unordered_map<char, Brbanje::Ref<Brbanje::SubTexture2D>> m_TileMap;

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;
	struct ProfileResult
	{
		const char* name;
		float time;
	};
	std::vector<ProfileResult> m_ProfileResults;
	
	

public:
	Sandbox2D();
	~Sandbox2D() = default;
	void OnAttach()	override;
	void OnDetach() override;
	void OnUpdate(Brbanje::Timestep ts)override;
	void OnEvent(Brbanje::Event& event)override;
	void OnImGuiRender()override;

};