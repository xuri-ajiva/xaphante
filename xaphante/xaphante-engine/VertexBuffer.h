#pragma once
#include <GL/glew.h>

#include "_defines.h"

struct VertexBuffer {
    
public:
    VertexBuffer(void* data, UInt32 num_vertices);

    virtual ~VertexBuffer();

    void Bind() const;

    static void Unbind();

private:
    GLuint BUFFER_ID_ {};
    GLuint VAO_ {};
};
