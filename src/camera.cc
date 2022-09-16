#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace rb
{

Camera::Camera(const glm::mat4& projection_matrix) : projection_matrix(projection_matrix)
{ }

void Camera::translate(const glm::vec3& delta_pos)
{
    this->position += delta_pos;
    this->dirty_view_matrix = true;
}

void Camera::increment_pitch(float delta_pitch)
{
    this->pitch += delta_pitch;
    this->pitch = glm::sign(this->pitch) * glm::min(glm::abs(this->pitch), 89.9f);
    this->dirty_look_at = true;
}

void Camera::increment_yaw(float delta_yaw)
{
    this->yaw += delta_yaw;
    this->dirty_look_at = true;
}

const glm::mat4& Camera::get_view_projection_matrix()
{
    this->refresh_if_needed();

    return this->view_projection_matrix;
}

void Camera::refresh_if_needed()
{
    if (this->dirty_look_at)
    {
        this->look_at = glm::normalize(glm::vec3(
            glm::cos(glm::radians(this->pitch)) * glm::cos(glm::radians(this->yaw)),
            glm::sin(glm::radians(this->pitch)),
            glm::cos(glm::radians(this->pitch)) * glm::sin(glm::radians(this->yaw))
        ));
        this->dirty_view_matrix = true;
        this->dirty_look_at = false;
    }

    if (this->dirty_view_matrix)
    {
        this->view_matrix = glm::lookAt(this->position, this->position + this->look_at, this->up);
        this->dirty_view_projection_matrix = true;
        this->dirty_view_matrix = false;
    }

    if (this->dirty_view_projection_matrix)
    {
        this->view_projection_matrix = this->projection_matrix * this->view_matrix;
        this->dirty_view_matrix = false;
    }
}

OrthographicCamera::OrthographicCamera(const Config& config)
  : Camera(glm::ortho(-config.aspect_ratio * config.zoom_level, config.aspect_ratio * config.zoom_level, -config.zoom_level, config.zoom_level))
{ }

IsometricCamera::IsometricCamera(const Config& config) : OrthographicCamera(config)
{
    this->increment_pitch(-31.5f);
    this->increment_yaw(45.0f);
}

PerspectiveCamera::PerspectiveCamera(const Config& config) : Camera(glm::perspective(config.fov / 2.0f, config.aspect_ratio, 0.1f, 1000.0f))
{ }

}  // namespace rb
