#pragma once
#include <GL/glew.h>
#include "_defines.h"

struct IndexBuffer {
	IndexBuffer(void* data, UInt32 numIndices, UInt8 elementSize);

	virtual ~IndexBuffer();

	void Bind() const;

	static void Unbind();

private:
	GLuint BUFFER_ID_ {};
};
