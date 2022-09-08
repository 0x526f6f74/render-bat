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
      : Camera(glm::perspective(config.fov / 2.0f, config.width / config.height, 0.1f, 1000.0f), config)
    {
        this->on_mouse_move({0.0f, 0.0f});
    }

    IsometricCamera::IsometricCamera(const CameraConfig& config, const glm::vec4& frame) noexcept
      : Camera(glm::ortho(frame.x, frame.y, frame.z, frame.w, -10.0f, 100.0f), config)
    {
        this->on_mouse_move({-150.0f, 105.0f});
    }

    void IsometricCameraController::update(double dt, const KeyboardState& keyboard)
    {
        if (keyboard.s_is_pressed) this->move_forwards(-dt * this->config.speed);
        if (keyboard.a_is_pressed) this->move_sideways(-dt * this->config.speed);
        if (keyboard.space_is_pressed) this->move_up(dt * this->config.speed);
        if (keyboard.shift_is_pressed) this->move_up(-dt * this->config.speed);
    }
}  // namespace rb
