#pragma once

#include <string>

#include "glad/glad.h"

namespace rb
{
    struct CubemapFaceTexturePaths
    {
        CubemapFaceTexturePaths() = default;
        CubemapFaceTexturePaths(const std::string& all_faces);
        CubemapFaceTexturePaths(
            const std::string& east, const std::string& west, const std::string& up, const std::string& down, const std::string& south, const std::string& north
        );

        std::string east, west, up, down, south, north;
    };

    class Cubemap
    {
    public:
        Cubemap(const CubemapFaceTexturePaths& texture_paths);

    private:
        GLuint id;

        void load_face_texture(const std::string& texture_path, int index) const;
    };
}  // namespace rb
