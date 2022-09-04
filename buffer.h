#pragma once

#include "glad/glad.h"

namespace rb
{
    class VertexBuffer
    {
    public:
        VertexBuffer(GLsizeiptr size, const void* data) noexcept;

        void bind() const noexcept;

    private:
        GLuint vao, vbo;
    };

    class IndexBuffer
    {
    public:
        IndexBuffer(GLsizeiptr size, const void* data) noexcept;

        void bind() const noexcept;

    private:
        GLuint ibo;
    };
}  // namespace rb
