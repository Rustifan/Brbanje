#pragma once
#include "Brbanje/Renderer/Framebuffer.h"

namespace Brbanje
{
	class OpenGlFramebuffer : public Framebuffer
	{
	private:
		FramebufferSpecifications m_FramebufferSpecs;
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
	public:
		OpenGlFramebuffer(const FramebufferSpecifications& framebufferSpecs);
		virtual ~OpenGlFramebuffer();
		virtual const FramebufferSpecifications& GetFramebufferSpecs() const override { return m_FramebufferSpecs; }
		void Invalidate();
		virtual void Bind()override;
		virtual void Unbind()override;
		virtual void Resize(uint32_t width, uint32_t height)override;
		virtual uint32_t GetColorAttachmentId()const override { return m_ColorAttachment; }
	};
}