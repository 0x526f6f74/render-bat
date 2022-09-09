#pragma once

#include <glm/glm.hpp>

#include "state.h"

namespace rb
{
    struct CameraConfig
    {
        float fov;
        int width, height;
        float speed = 3.0f;
        float mouse_sensivity = 0.3f;
    };

    class Camera
    {
    public:
        Camera(const glm::mat4& projection_matrix, const CameraConfig& config);

        void translate(const glm::vec3& v) noexcept;
        void on_mouse_move(const glm::vec2& delta_pos) noexcept;

        void move_forwards(float distance) noexcept;
        void move_sideways(float distance) noexcept;
        void move_up(float distance) noexcept;

        const glm::mat4& get_view_projection_matrix() noexcept;
        const glm::mat4& get_view_matrix() noexcept;

    protected:
        glm::mat4 projection_matrix;
        bool dirty_matrices = false;
        const CameraConfig config;

    private:
        glm::vec3 position;
        glm::vec3 look_at;
        glm::vec3 up;
        glm::mat4 view_matrix;
        glm::mat4 view_projection_matrix;
        float yaw, pitch;

        void refresh_matrices() noexcept;
    };

    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(const CameraConfig& config) noexcept;
    };

    class IsometricCamera : public Camera
    {
    public:
        IsometricCamera(const CameraConfig& config, float aspect_ratio, float zoom_level) noexcept;

        void set_zoom_level(float zoom_level) noexcept;

    private:
        float aspect_ratio;
    };

    class IsometricCameraController : public IsometricCamera
    {
    public:
        using IsometricCamera::IsometricCamera;

        void update(const State& state, bool cursor_is_disabled);
    };
}  // namespace rb
