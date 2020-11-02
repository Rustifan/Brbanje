#pragma once

#include "Brbanje/Core/Core.h"
#include "Brbanje/Core/Log.h"


namespace Brbanje
{
	enum class ShaderDataType
	{
		none = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:				return 4;
		case ShaderDataType::Float2:			return 4 * 2;
		case ShaderDataType::Float3:			return 4 * 3;
		case ShaderDataType::Float4:			return 4 * 4;
		case ShaderDataType::Mat3:				return 4 * 3 * 3;
		case ShaderDataType::Mat4:				return 4 * 4 * 4;
		case ShaderDataType::Int:				return 4;
		case ShaderDataType::Int2:				return 4 * 2;
		case ShaderDataType::Int3:				return 4 * 3;
		case ShaderDataType::Int4:				return 4 * 4;
		case ShaderDataType::Bool:				return 1;
		
		}

		BR_CORE_ASSERT(false, "Unown data type")
			return 0;

	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		
		uint32_t Size;

		bool normalized = false;

		BufferElement(ShaderDataType type, std::string name, bool normalized = false) :
			Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), normalized(normalized)
		{
		}

		uint32_t GetComponentCount()const
		{
			switch (Type)
			{

				case ShaderDataType::Float:				return 1;
				case ShaderDataType::Float2:			return 2;
				case ShaderDataType::Float3:			return 3;
				case ShaderDataType::Float4:			return 4;
				case ShaderDataType::Mat3:				return 3 * 3;
				case ShaderDataType::Mat4:				return 4 * 4;
				case ShaderDataType::Int:				return 1;
				case ShaderDataType::Int2:				return 2;
				case ShaderDataType::Int3:				return 3;
				case ShaderDataType::Int4:				return 4;
				case ShaderDataType::Bool:				return 1;

				

			}
			BR_CORE_ASSERT(false, "Unown data type")
			return 0;
			
		}
	};


	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_BufferElements;
		uint32_t stride = 0;
		
	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			stride = 0;
			
			for (auto& element : m_BufferElements)
			{
				element.Offset = offset;
				offset += element.Size;
				stride += element.Size;
			}
			
		}
	public:
		BufferLayout(){}
		BufferLayout(const std::initializer_list<BufferElement>& elements): m_BufferElements(elements)
		{
			CalculateOffsetAndStride();
		}
		inline const std::vector<BufferElement>& GetBufferEllements()const { return  m_BufferElements; }
		inline uint32_t GetStride() const { return stride; }
		
		std::vector<BufferElement>::iterator begin() { return m_BufferElements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_BufferElements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_BufferElements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_BufferElements.end(); }
	};









	class VertexBuffer
	{
	public:
		
		virtual ~VertexBuffer() {}
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
		static Ref<VertexBuffer> Create(uint32_t size);
		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout()const = 0;

	};


	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual uint32_t GetCount()const = 0;
		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);


	};
}