#pragma once

#include <glm/glm.hpp>

namespace rb
{

class Camera
{
public:
    struct Config
    {
        float aspect_ratio;
    };

    Camera(const glm::mat4& projection_matrix);

    void translate(const glm::vec3& delta_pos);
    void increment_pitch(float delta_pitch);
    void increment_yaw(float delta_yaw);

    const glm::mat4& get_view_projection_matrix();

protected:
    glm::mat4 projection_matrix;

    bool dirty_view_projection_matrix = true;

private:
    static constexpr glm::vec3 up{0.0f, 1.0f, 0.0f};

    void refresh_if_needed();

    float pitch = 0.0f;
    float yaw = -90.0f;
    glm::vec3 position{0.0f};
    glm::vec3 look_at;
    glm::mat4 view_matrix;
    glm::mat4 view_projection_matrix;

    bool dirty_look_at = true;
    bool dirty_view_matrix = true;
};

class OrthographicCamera : public Camera
{
public:
    struct Config : public Camera::Config
    {
        float zoom_level;
    };

    OrthographicCamera(const Config& config);
};

class IsometricCamera : public OrthographicCamera
{
public:
    IsometricCamera(const Config& config);
};

class PerspectiveCamera : public Camera
{
public:
    struct Config : public Camera::Config
    {
        float fov;
    };

    PerspectiveCamera(const Config& config);
};

}  // namespace rb
