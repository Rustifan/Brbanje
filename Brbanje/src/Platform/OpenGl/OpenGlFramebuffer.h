#pragma once
#include "Brbanje/Renderer/Framebuffer.h"

namespace Brbanje
{
	class OpenGlFramebuffer : public Framebuffer
	{
	private:
		FramebufferSpecifications m_FramebufferSpecs;
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
	public:
		OpenGlFramebuffer(const FramebufferSpecifications& framebufferSpecs);
		virtual ~OpenGlFramebuffer();
		virtual const FramebufferSpecifications& GetFramebufferSpecs() const override { return m_FramebufferSpecs; }
		void Invalidate();
		virtual void Bind()override;
		virtual void Unbind()override;
		virtual uint32_t GetColorAttachmentId()const override { return m_ColorAttachment; }
	};
}