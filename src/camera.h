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
    void move_forwards(float distance);
    void move_sideways(float distance);
    void move_upwards(float distance);

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

    void increment_zoom_level(float delta_zoom);

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

    void increment_zoom_level(float delta_zoom);

private:
    Config config;
};

template<typename T>
class CameraController : public T
{
public:
    struct Config
    {
        T::Config base;

        float velocity;
        float mouse_sensivity;
        float zoom_sensivity;
    };

    struct KeyboardState
    {
        bool w = false;
        bool a = false;
        bool s = false;
        bool d = false;
        bool space = false;
        bool shift = false;
    };

    CameraController(const Config& config) : T(config.base), config(config)
    { }

    void update_position(double dt, const KeyboardState& keyboard)
    {
        if (keyboard.w) this->move_forwards(this->config.velocity * dt);
        if (keyboard.a) this->move_sideways(-this->config.velocity * dt);
        if (keyboard.s) this->move_forwards(-this->config.velocity * dt);
        if (keyboard.d) this->move_sideways(this->config.velocity * dt);
        if (keyboard.space) this->move_upwards(this->config.velocity * dt);
        if (keyboard.shift) this->move_upwards(-this->config.velocity * dt);
    }

    void on_mouse_moved(const glm::vec2& delta_pos)
    {
        this->increment_pitch(-delta_pos.y * this->config.mouse_sensivity);
        this->increment_yaw(delta_pos.x * this->config.mouse_sensivity);
    }

    void on_mouse_scrolled(double dy)
    {
        this->increment_zoom_level(-dy * this->config.zoom_sensivity);
    }

private:
    Config config;
};

using OrthographicCameraController = CameraController<OrthographicCamera>;
using IsometricCameraController = CameraController<IsometricCamera>;
using PerspectiveCameraController = CameraController<PerspectiveCamera>;

}  // namespace rb
