#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace rb
{
    Camera::Camera(const glm::mat4& projection_matrix, const CameraConfig& config)
      : projection_matrix(projection_matrix)
      , view_matrix(glm::mat4(1.0f))
      , position(glm::vec3(0.0f))
      , up(glm::vec3(0.0f, 1.0f, 0.0f))
      , yaw(-90.0f)
      , pitch(0.0f)
      , config(config)
    { }

    void Camera::translate(const glm::vec3& v) noexcept
    {
        this->position += v;
        this->dirty_matrices = true;
    }

    void Camera::on_mouse_move(const glm::vec2& delta_pos) noexcept
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

    void Camera::refresh_matrices() noexcept
    {
        this->view_matrix = glm::lookAt(this->position, this->position + this->look_at, this->up);
        this->view_projection_matrix = this->projection_matrix * this->view_matrix;
        this->dirty_matrices = false;
    }

    void Camera::move_forwards(float distance) noexcept
    {
        this->translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * this->look_at) * distance);
    }

    void Camera::move_sideways(float distance) noexcept
    {
        this->translate(glm::normalize(glm::cross(this->look_at, this->up)) * distance);
    }

    void Camera::move_up(float distance) noexcept
    {
        this->translate(this->up * distance);
    }

    const glm::mat4& Camera::get_view_projection_matrix() noexcept
    {
        if (dirty_matrices) this->refresh_matrices();

        return this->view_projection_matrix;
    }

    const glm::mat4& Camera::get_view_matrix() noexcept
    {
        if (dirty_matrices) this->refresh_matrices();

        return this->view_matrix;
    }

    PerspectiveCamera::PerspectiveCamera(const CameraConfig& config) noexcept
      : Camera(glm::perspective(config.fov / 2.0f, static_cast<float>(config.width) / config.height, 0.1f, 1000.0f), config)
    {
        this->on_mouse_move({0.0f, 0.0f});
    }

    IsometricCamera::IsometricCamera(const CameraConfig& config, float aspect_ratio, float zoom_level) noexcept
      : Camera(glm::ortho(-aspect_ratio * zoom_level, aspect_ratio * zoom_level, -zoom_level, zoom_level, -10.0f, 100.0f), config), aspect_ratio(aspect_ratio)
    {
        this->on_mouse_move({-150.0f, 105.0f});
    }

    void IsometricCamera::set_zoom_level(float zoom_level) noexcept
    {
        this->projection_matrix = glm::ortho(-this->aspect_ratio * zoom_level, this->aspect_ratio * zoom_level, -zoom_level, zoom_level, -10.0f, 100.0f);
        this->dirty_matrices = true;
    }

    void IsometricCameraController::update(const State& state, bool cursor_is_disabled)
    {
        if (state.keyboard.a_is_pressed) this->move_sideways(-state.time.delta * this->config.speed);
        if (state.keyboard.d_is_pressed) this->move_sideways(state.time.delta * this->config.speed);
        if (state.keyboard.space_is_pressed) this->move_up(state.time.delta * this->config.speed);
        if (state.keyboard.shift_is_pressed) this->move_up(-state.time.delta * this->config.speed);

        if (cursor_is_disabled && state.mouse.delta_pos != glm::dvec2(0.0f)) this->on_mouse_move(state.mouse.delta_pos);
    }
}  // namespace rb