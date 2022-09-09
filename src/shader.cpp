#include "shader.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

namespace rb
{
    static constexpr char TYPE_DIRECTIVE[] = "#type";
    static constexpr char INCLUSION_DIRECTIVE[] = "#include";
    static constexpr char INCLUSION_DIRECTORY[] = "../shaders/include/";
    static constexpr int MAX_SHADER_LEN = 8192;
    static constexpr int MAX_NUM_SHADERS = 4;
    static constexpr int MAX_SHADER_LINE_LEN = 256;

    namespace utils
    {
        static GLenum shader_stage_string_to_opengl(const char* stage)
        {
            if (!std::strcmp(stage, "vertex"))
                return GL_VERTEX_SHADER;

            else if (!std::strcmp(stage, "geometry"))
                return GL_GEOMETRY_SHADER;

            else if (!std::strcmp(stage, "fragment"))
                return GL_FRAGMENT_SHADER;

            else if (!std::strcmp(stage, "compute"))
                return GL_COMPUTE_SHADER;

            std::cerr << "shader_stage_string_to_opengl: unknown shader stage\n";
            return 0;
        }
    }  // namespace utils

    Shader::Shader(const char* filepath)
    {
        const auto sources = this->read_from_file(filepath);
        this->compile(sources);

        for (const auto& [stage, source] : sources) delete[] source;
    }

    Shader::~Shader()
    {
        glDeleteProgram(this->id);
    }

    void Shader::bind() const noexcept
    {
        glUseProgram(this->id);
    }

    void Shader::unbind() const noexcept
    {
        glUseProgram(0);
    }

    std::unordered_map<GLenum, char*> Shader::read_from_file(const char* filepath) const
    {
        std::unordered_map<GLenum, char*> sources;

        std::FILE* file = std::fopen(filepath, "r");

        if (!file)
        {
            std::cerr << "Shader not found error: " << filepath << '\n';
            return sources;
        }

        char line[MAX_SHADER_LINE_LEN];
        GLenum stage;

        while (std::fgets(line, MAX_SHADER_LINE_LEN, file))
        {
            if (!std::strncmp(line, TYPE_DIRECTIVE, std::strlen(TYPE_DIRECTIVE)))
            {
                line[std::strcspn(line, "\n")] = '\0';
                stage = utils::shader_stage_string_to_opengl(line + std::strlen(TYPE_DIRECTIVE) + 1);
                sources[stage] = new char[MAX_SHADER_LEN];
                char*& source = sources[stage];
                std::memset(source, 0, MAX_SHADER_LEN);
                continue;
            }

            else if (!std::strncmp(line, INCLUSION_DIRECTIVE, std::strlen(INCLUSION_DIRECTIVE)))
            {
                char inclusion_source[MAX_SHADER_LEN] = {0};
                this->read_inclusion_from_file(inclusion_source, line);
                std::strcat(sources[stage], inclusion_source);
                continue;
            }

            std::strcat(sources[stage], line);
        }

        std::fclose(file);

        return sources;
    }

    void Shader::read_inclusion_from_file(char* source, char* inclusion_line) const
    {
        inclusion_line[std::strcspn(inclusion_line, "\n")] = '\0';
        char filepath[MAX_SHADER_LINE_LEN];
        std::strcpy(filepath, INCLUSION_DIRECTORY);
        std::strcat(filepath, inclusion_line + std::strlen(INCLUSION_DIRECTIVE) + 1);

        std::FILE* file = std::fopen(filepath, "r");

        if (file == nullptr)
        {
            std::cerr << "Inclusion shader not found error: " << filepath << '\n';
            return;
        }

        char line[MAX_SHADER_LINE_LEN];

        while (std::fgets(line, MAX_SHADER_LINE_LEN, file))
        {
            if (!std::strncmp(line, INCLUSION_DIRECTIVE, std::strlen(INCLUSION_DIRECTIVE)))
            {
                this->read_inclusion_from_file(source, line);
                continue;
            }

            std::strcat(source, line);
        }

        std::fclose(file);
    }

    void Shader::compile(const std::unordered_map<GLenum, char*>& sources)
    {
        this->id = glCreateProgram();

        GLuint shader_ids[MAX_NUM_SHADERS];
        const int num_shaders = sources.size();
        int i = 0;

        for (const auto& [stage, source] : sources)
        {
            const GLuint shader_id = glCreateShader(stage);
            glShaderSource(shader_id, 1, &source, nullptr);
            glCompileShader(shader_id);

            GLint compile_status = 0;
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);

            if (compile_status == GL_FALSE)
            {
                GLint info_log_length;
                glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

                std::vector<GLchar> info_log(info_log_length);
                glGetShaderInfoLog(shader_id, info_log_length, &info_log_length, info_log.data());
                std::cerr << "Shader compilation error:\n" << info_log.data();
                glDeleteShader(shader_id);

                break;
            }

            glAttachShader(this->id, shader_id);
            shader_ids[i++] = shader_id;
        }

        glLinkProgram(this->id);

        GLint link_status = 0;
        glGetProgramiv(this->id, GL_LINK_STATUS, &link_status);

        if (link_status == GL_FALSE)
        {
            GLint info_log_length;
            glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &info_log_length);

            std::vector<GLchar> info_log;
            glGetProgramInfoLog(this->id, info_log_length, &info_log_length, info_log.data());
            std::cerr << "Shader linking error:\n" << info_log.data();

            glDeleteProgram(this->id);

            for (i = 0; i < num_shaders; ++i) glDeleteShader(shader_ids[i]);
        }

        for (i = 0; i < num_shaders; ++i)
        {
            glDetachShader(this->id, shader_ids[i]);
            glDeleteShader(shader_ids[i]);
        }
    }

    void Shader::set_uniform_matrix4fv(const char* name, const glm::mat4& value) const noexcept
    {
        glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, &value[0][0]);
    }

    void Shader::set_uniform_1i(const char* name, int value) const noexcept
    {
        glUniform1i(glGetUniformLocation(this->id, name), value);
    }

    void Shader::set_uniform_1iv(const char* name, GLsizei count, const GLint* value) const noexcept
    {
        glUniform1iv(glGetUniformLocation(this->id, name), count, value);
    }
}  // namespace rb
