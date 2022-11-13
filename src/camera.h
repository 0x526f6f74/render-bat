#pragma once

#include "state.h"

namespace rb
{

class Camera
{
public:
    struct Config
    {
        Config() = default;
        Config(int viewport_width, int viewport_height);

        int viewport_width;
        int viewport_height;
    };

    Camera(const glm::mat4& projection_matrix);

    void move_forwards(float distance);
    void move_sideways(float distance);
    void move_upwards(float distance);

    void increment_pitch(float delta_pitch);
    void increment_yaw(float delta_yaw);

    virtual void zoom_in(float zoom_level) = 0;

    const glm::mat4& get_view_projection_matrix();

protected:
    glm::mat4 projection_matrix;

    bool dirty_view_projection_matrix = true;

private:
    static constexpr glm::vec3 up {0.0f, 1.0f, 0.0f};

    void translate(const glm::vec3& delta_pos);
    void refresh_if_needed();

    float pitch = 0.0f;
    float yaw = -90.0f;
    glm::vec3 position {0.0f};
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
        Config() = default;
        Config(int viewport_width, int viewport_height, float zoom_level);
        Config(float aspect_ratio, float zoom_level);

        float aspect_ratio;
        float zoom_level;
    };

    OrthographicCamera(const Config& config);

    virtual void zoom_in(float delta_zoom) override;

private:
    Config config;
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

    virtual void zoom_in(float delta_zoom) override;

private:
    Config config;
};

class CameraController
{
public:
    struct Config
    {
        float velocity;
        float mouse_sensivity;
        float zoom_sensivity;
    };

    CameraController(const Config& config, Camera* camera);

    void update(double dt, const KeyboardState& keyboard) const;
    void on_mouse_moved(const glm::vec2& delta_pos) const;
    void on_mouse_scrolled(double dy) const;

private:
    Config config;
    Camera* camera;
};

}  // namespace rb
