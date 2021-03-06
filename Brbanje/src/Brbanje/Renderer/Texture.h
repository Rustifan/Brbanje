#pragma once

#include "Brbanje/Core/Core.h"

namespace Brbanje
{
	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void Bind(uint32_t slot) const = 0;
		virtual void SetData(void* data, uint32_t size) const = 0;
		virtual bool operator==(const Texture& other) = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual const std::string& GetFilePath() const = 0;
	
	};

	class Texture2D : public Texture
	{
		
	public:
		static Ref<Texture2D> Create(const std::string& fileName);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		
	};




}