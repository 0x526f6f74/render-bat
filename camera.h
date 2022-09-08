#pragma once

#include <glm/glm.hpp>

#include "state.h"

namespace rb
{
    struct CameraConfig
    {
        float fov;
        float width, height;
        float speed = 3.0f;
        float mouse_sensivity = 0.3f;
    };

    class Camera
    {
    public:
        Camera(const glm::mat4& projection_matrix, const CameraConfig& config);

        virtual void translate(const glm::vec3& v) noexcept;
        virtual void on_mouse_move(const glm::vec2& delta_pos) noexcept;

        virtual void move_forwards(float distance) noexcept;
        virtual void move_sideways(float distance) noexcept;
        virtual void move_up(float distance) noexcept;

        virtual const glm::mat4& get_view_projection_matrix() noexcept;
        virtual const glm::mat4& get_view_matrix() noexcept;

    protected:
        glm::vec3 position;
        glm::vec3 look_at;
        glm::vec3 up;
        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;
        glm::mat4 view_projection_matrix;
        const CameraConfig config;

        float yaw, pitch;
        bool dirty_matrices = false;

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
        IsometricCamera(const CameraConfig& config, const glm::vec4& frame) noexcept;
    };

    class IsometricCameraController : public IsometricCamera
    {
    public:
        using IsometricCamera::IsometricCamera;

        void update(double dt, const KeyboardState& keyboard);
    };
}  // namespace rb
