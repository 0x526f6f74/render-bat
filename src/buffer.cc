#include "buffer.h"

#include "vertex.h"

namespace rb
{
    VertexBuffer::VertexBuffer(GLsizeiptr size, const void* data)
    {
        glGenVertexArrays(1, &this->vao);
        glBindVertexArray(this->vao);

        glGenBuffers(1, &this->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, im_coords));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texture_index));
    }

    void VertexBuffer::bind() const
    {
        glBindVertexArray(this->vao);
    }

    IndexBuffer::IndexBuffer(GLsizeiptr size, const void* data)
    {
        glGenBuffers(1, &this->ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    }
}  // namespace rb
