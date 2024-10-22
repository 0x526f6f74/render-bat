#pragma once

#include "glad/glad.h"

namespace rb
{

class Framebuffer
{
public:
    Framebuffer(int width, int height);
    ~Framebuffer();

private:
    GLuint fbo, color_rbo, depth_rbo;
};

void write_color_buffer_to_png_file(const char* filepath, int width, int height);

}  // namespace rb
