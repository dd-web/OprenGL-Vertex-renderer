#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count, T type, bool normalize)
	{
		m_Elements.push_back({ (unsigned int)type, count, (unsigned char)normalize });
		m_Stride += count * VertexBufferElement::GetSizeOfType(type);
	}

	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; };
	inline const unsigned int GetStride() const { return m_Stride;  }
};