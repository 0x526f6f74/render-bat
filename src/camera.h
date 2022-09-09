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

        void translate(const glm::vec3& v);
        void on_mouse_move(const glm::vec2& delta_pos);

        void move_forwards(float distance);
        void move_sideways(float distance);
        void move_up(float distance);

        const glm::mat4& get_view_projection_matrix();
        const glm::mat4& get_view_matrix();

    protected:
        const CameraConfig config;

        glm::mat4 projection_matrix;
        bool dirty_matrices = false;

    private:
        glm::vec3 position;
        glm::vec3 look_at;
        glm::vec3 up;
        glm::mat4 view_matrix;
        glm::mat4 view_projection_matrix;
        float yaw, pitch;

        void refresh_matrices();
    };

    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(const CameraConfig& config);
    };

    class IsometricCamera : public Camera
    {
    public:
        IsometricCamera(const CameraConfig& config, float aspect_ratio, float zoom_level);

        void set_zoom_level(float zoom_level);

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
