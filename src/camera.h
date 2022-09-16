#pragma once

#include <glm/glm.hpp>

#include "window.h"

namespace rb
{

struct CameraConfig
{
    int width, height;
    float aspect_ratio;

    float speed;
    float mouse_sensivity;
    float zoom_sensivity;
};

struct OrthographicCameraConfig
{
    CameraConfig camera;

    float zoom_level;
};

struct PerspectiveCameraConfig
{
    CameraConfig camera;

    float fov;
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

    void set_projection_matrix(const glm::mat4& projection_matrix);

private:
    glm::mat4 projection_matrix;
    glm::mat4 view_matrix{1.0f};
    glm::mat4 view_projection_matrix;
    bool dirty_matrices = false;

    glm::vec3 position{0.0f};
    glm::vec3 look_at;
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    float yaw = -90.0f;
    float pitch = 0.0f;

    void refresh_matrices();
};

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(const OrthographicCameraConfig& config);

    void on_mouse_scroll(double yoffset);

protected:
    OrthographicCameraConfig config;
};

class IsometricCamera : public OrthographicCamera
{
public:
    IsometricCamera(const OrthographicCameraConfig& config);
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(const PerspectiveCameraConfig& config);
};

class IsometricCameraController : public IsometricCamera
{
public:
    using IsometricCamera::IsometricCamera;

    void update(const RealtimeWindowState& state);
};

}  // namespace rb
