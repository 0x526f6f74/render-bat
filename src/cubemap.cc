#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "cubemap.h"
#include "stb/stb_image.h"

namespace rb
{

CubemapFaceTexturePaths::CubemapFaceTexturePaths(
    const std::string& east, const std::string& west, const std::string& up, const std::string& down, const std::string& south, const std::string& north
)
  : east(east), west(west), up(up), down(down), south(south), north(north)
{ }

CubemapFaceTexturePaths::CubemapFaceTexturePaths(const std::string& all_faces)
  : east(all_faces), west(all_faces), up(all_faces), down(all_faces), south(all_faces), north(all_faces)
{ }

Cubemap::Cubemap(const CubemapFaceTexturePaths& texture_paths)
{
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // TODO: prevent repeatedly loading the same textures

    this->load_face_texture(texture_paths.east, 0);
    this->load_face_texture(texture_paths.west, 1);
    this->load_face_texture(texture_paths.up, 2);
    this->load_face_texture(texture_paths.down, 3);
    this->load_face_texture(texture_paths.south, 4);
    this->load_face_texture(texture_paths.north, 5);
}

void Cubemap::load_face_texture(const std::string& texture_path, int index) const
{
    int width, height, num_channels;
    stbi_uc* data = stbi_load(texture_path.c_str(), &width, &height, &num_channels, 0);

    if (data == nullptr)
    {
        std::cerr << "Failed to load cubemap face texture: file \"" << texture_path << "\" was not found\n";
        stbi_image_free(data);
        return;
    }

    GLenum internal_format, data_format = 0;

    switch (num_channels)
    {
        case 4:
            internal_format = GL_RGBA8;
            data_format = GL_RGBA;
            break;

        case 3:
            internal_format = GL_RGB8;
            data_format = GL_RGB;
            break;
    }

    if (!(internal_format & data_format))
    {
        std::cerr << "Failed to load cubemap face texture: unknown internal/data format\n";
        stbi_image_free(data);
        return;
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

}  // namespace rb
