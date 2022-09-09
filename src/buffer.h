#pragma once

#include "glad/glad.h"

namespace rb
{
    class VertexBuffer
    {
    public:
        VertexBuffer(GLsizeiptr size, const void* data);

        void bind() const;

    private:
        GLuint vao, vbo;
    };

    class IndexBuffer
    {
    public:
        IndexBuffer(GLsizeiptr size, const void* data);

        void bind() const;

    private:
        GLuint ibo;
    };
}  // namespace rb
