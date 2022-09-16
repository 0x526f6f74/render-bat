#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace rb
{

Camera::Camera(const CameraConfig& config, const glm::mat4& projection_matrix) : config(config), projection_matrix(projection_matrix)
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
        glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw)), glm::sin(glm::radians(pitch)), glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw))
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

OrthographicCamera::OrthographicCamera(const OrthographicCameraConfig& config)
  : Camera(
        config.camera,
        glm::ortho(
            -config.camera.aspect_ratio * config.zoom_level,
            config.camera.aspect_ratio * config.zoom_level,
            -config.zoom_level,
            config.zoom_level,
            -10.0f,
            100.0f
        )
    )
  , config(config)
{ }

void OrthographicCamera::on_mouse_scroll(double yoffset)
{
    this->config.zoom_level -= yoffset * this->config.camera.zoom_sensivity;
    this->projection_matrix = glm::ortho(
        -this->config.camera.aspect_ratio * this->config.zoom_level,
        this->config.camera.aspect_ratio * this->config.zoom_level,
        -this->config.zoom_level,
        this->config.zoom_level,
        -10.0f,
        100.0f
    );
    this->dirty_matrices = true;
}

IsometricCamera::IsometricCamera(const OrthographicCameraConfig& config) : OrthographicCamera(config)
{
    this->on_mouse_move({-150.0f, 105.0f});
}

PerspectiveCamera::PerspectiveCamera(const PerspectiveCameraConfig& config)
  : Camera(config.camera, glm::perspective(config.fov / 2.0f, config.camera.aspect_ratio, 0.1f, 1000.0f))
{
    this->on_mouse_move({0.0f, 0.0f});
}

void IsometricCameraController::update(const RealtimeWindowState& state)
{
    if (state.a_is_pressed) this->move_sideways(-state.dt * this->config.camera.speed);
    if (state.d_is_pressed) this->move_sideways(state.dt * this->config.camera.speed);
    if (state.space_is_pressed) this->move_up(state.dt * this->config.camera.speed);
    if (state.shift_is_pressed) this->move_up(-state.dt * this->config.camera.speed);
}

}  // namespace rb
