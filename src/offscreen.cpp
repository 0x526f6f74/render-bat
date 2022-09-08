#include "offscreen.h"

#include <png.h>

#include <fstream>
#include <iostream>

namespace rb
{
    Framebuffer::Framebuffer(int width, int height) noexcept
    {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

        glGenRenderbuffers(1, &this->color_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, this->color_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB565, width, height);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->color_rbo);

        glGenRenderbuffers(1, &this->depth_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, this->depth_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depth_rbo);

        glReadBuffer(GL_COLOR_ATTACHMENT0);
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &this->fbo);
        glDeleteRenderbuffers(1, &this->color_rbo);
        glDeleteRenderbuffers(1, &this->depth_rbo);
    }

    void write_color_buffer_to_png_file(const char* filename, int width, int height)
    {
        png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png)
        {
            std::cerr << "Failed to create png write struct!\n";
            return;
        }
        if (setjmp(png_jmpbuf(png)))
        {
            std::cerr << "PNG jump buffer error!\n";
            return;
        }

        png_infop info = png_create_info_struct(png);
        if (!info)
        {
            std::cerr << "Failed to create png info struct!\n";
            return;
        }

        constexpr int num_channels = 4;
        const int image_size = num_channels * width * height;

        png_byte* png_bytes = new png_byte[image_size];
        png_byte** png_rows = new png_byte*[height];

        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, png_bytes);

        for (int i = 0; i < height; ++i)
            png_rows[height - i - 1] = &png_bytes[i * width * num_channels];

        std::FILE* file = std::fopen(filename, "wb");

        png_init_io(png, file);
        png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        png_write_info(png, info);
        png_write_image(png, png_rows);
        png_write_end(png, nullptr);
        png_destroy_write_struct(&png, &info);

        std::fclose(file);

        delete[] png_bytes;
        delete[] png_rows;
    }
}  // namespace rb
