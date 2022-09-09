#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace rb
{
    CameraConfig::CameraConfig(int width, int height)
      : width(width), height(height), fov(90.0f), speed(3.0f), mouse_sensivity(0.3f), aspect_ratio(static_cast<float>(width) / height)
    { }

    Camera::Camera(const glm::mat4& projection_matrix, const CameraConfig& config)
      : config(config)
      , projection_matrix(projection_matrix)
      , dirty_matrices(false)
      , position(glm::vec3(0.0f))
      , up(glm::vec3(0.0f, 1.0f, 0.0f))
      , view_matrix(glm::mat4(1.0f))
      , yaw(-90.0f)
      , pitch(0.0f)
    { }

    void Camera::translate(const glm::vec3& v)
    {
        this->position += v;
        this->dirty_matrices = true;
    }

    void Camera::on_mouse_move(const glm::vec2& delta_pos)
    {
        this->yaw += delta_pos.x * this->config.mouse_sensivity;
        this->pitch -= delta_pos.y * this->config.mouse_sensivity;
        this->pitch = glm::sign(this->pitch) * glm::min(glm::abs(this->pitch), 89.0f);

        this->look_at = glm::normalize(glm::vec3(
            glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw)),
            glm::sin(glm::radians(pitch)),
            glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw))
        ));
        this->dirty_matrices = true;
    }

    void Camera::refresh_matrices()
    {
        this->view_matrix = glm::lookAt(this->position, this->position + this->look_at, this->up);
        this->view_projection_matrix = this->projection_matrix * this->view_matrix;
        this->dirty_matrices = false;
    }

    void Camera::move_forwards(float distance)
    {
        this->translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * this->look_at) * distance);
    }

    void Camera::move_sideways(float distance)
    {
        this->translate(glm::normalize(glm::cross(this->look_at, this->up)) * distance);
    }

    void Camera::move_up(float distance)
    {
        this->translate(this->up * distance);
    }

    const glm::mat4& Camera::get_view_projection_matrix()
    {
        if (dirty_matrices) this->refresh_matrices();

        return this->view_projection_matrix;
    }

    const glm::mat4& Camera::get_view_matrix()
    {
        if (dirty_matrices) this->refresh_matrices();

        return this->view_matrix;
    }

    PerspectiveCamera::PerspectiveCamera(const CameraConfig& config)
      : Camera(glm::perspective(config.fov / 2.0f, static_cast<float>(config.width) / config.height, 0.1f, 1000.0f), config)
    {
        this->on_mouse_move({0.0f, 0.0f});
    }

    IsometricCamera::IsometricCamera(const CameraConfig& config, float zoom_level)
      : Camera(glm::ortho(-config.aspect_ratio * zoom_level, config.aspect_ratio * zoom_level, -zoom_level, zoom_level, -10.0f, 100.0f), config)
    {
        this->on_mouse_move({-150.0f, 105.0f});
    }

    void IsometricCamera::set_zoom_level(float zoom_level)
    {
        this->projection_matrix =
            glm::ortho(-this->config.aspect_ratio * zoom_level, this->config.aspect_ratio * zoom_level, -zoom_level, zoom_level, -10.0f, 100.0f);
        this->dirty_matrices = true;
    }

    void IsometricCameraController::update(const RealtimeWindowState& state)
    {
        if (state.a_is_pressed) this->move_sideways(-state.dt * this->config.speed);
        if (state.d_is_pressed) this->move_sideways(state.dt * this->config.speed);
        if (state.space_is_pressed) this->move_up(state.dt * this->config.speed);
        if (state.shift_is_pressed) this->move_up(-state.dt * this->config.speed);

        if (state.cursor_is_disabled && state.delta_pos != glm::dvec2(0.0f)) this->on_mouse_move(state.delta_pos);
    }
}  // namespace rb
