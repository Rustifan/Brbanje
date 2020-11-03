#pragma once

namespace Brbanje
{
	struct FramebufferSpecifications
	{
		uint32_t width, height;

		uint32_t samples = 1;

		bool swapChainTarget = false;
	};
	
	class Framebuffer
	{
	public:
		virtual const FramebufferSpecifications& GetFramebufferSpecs() const = 0;


		static Ref<Framebuffer> Create(const FramebufferSpecifications& frameBufferSpecs);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual uint32_t GetColorAttachmentId() const = 0;
	};
}
