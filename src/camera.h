#pragma once

#include <glm/glm.hpp>

#include "window.h"

namespace rb
{

struct CameraConfig
{
    CameraConfig() = default;
    CameraConfig(int width, int height);

    int width, height;
    float aspect_ratio;

    float fov;

    float speed;
    float mouse_sensivity;
    float zoom_sensivity;
};

class Camera
{
public:
    Camera(const CameraConfig& config, const glm::mat4& projection_matrix);

    void translate(const glm::vec3& v);
    void on_mouse_move(const glm::vec2& delta_pos);

    void move_forwards(float distance);
    void move_sideways(float distance);
    void move_up(float distance);

    const glm::mat4& get_view_projection_matrix();

protected:
    const CameraConfig config;

    glm::mat4 projection_matrix;
    bool dirty_matrices = false;

private:
    glm::vec3 position{0.0f};
    glm::vec3 look_at;
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::mat4 view_matrix{1.0f};
    glm::mat4 view_projection_matrix;
    float yaw = -90.0f, pitch = 0.0f;

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
    IsometricCamera(const CameraConfig& config, float zoom_level);

    void on_mouse_scroll(double yoffset);

private:
    float zoom_level;
};

class IsometricCameraController : public IsometricCamera
{
public:
    using IsometricCamera::IsometricCamera;

    void update(const RealtimeWindowState& state);
};

}  // namespace rb
