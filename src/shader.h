#pragma once

#include "glad/glad.h"

namespace rb
{

class Shader
{
public:
    Shader(const char* filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void set_uniform_int(const char* name, int value) const;
    void set_uniform_int_array(const char* name, int count, const int* value) const;
    void set_uniform_mat4(const char* name, const glm::mat4& value) const;

private:
    std::unordered_map<GLenum, char*> read_from_file(const char* filepath) const;
    void read_inclusion_from_file(char* source, char* inclusion_line) const;
    void compile(const std::unordered_map<GLenum, char*>& sources);

    GLuint id;
};

}  // namespace rb
