#pragma once

#include "GLFW/glfw3.h"
#include "state.h"

namespace rb
{

class RealtimeWindow
{
public:
    struct Config
    {
        std::string title;
        int width;
        int height;
        bool vsync;

        int context_version_major;
        int context_version_minor;
        int num_samples;

        std::function<void(RealtimeWindow&, double, double)> scroll_callback;
        std::function<void(RealtimeWindow&, double, double)> cursor_pos_callback;
    };

    struct State
    {
        const Config config;
        KeyboardState keyboard;

        double time;
        double dt;

        glm::vec2 cursor_pos;
        bool cursor_is_disabled = false;
    };

    RealtimeWindow(const Config& config);
    ~RealtimeWindow();

    void update();
    void swap_buffers() const;

    bool is_open() const;
    void close() const;
    void set_input_mode(int mode, int value) const;

    State& get_state();
    const State& get_state() const;

private:
    State state;
    GLFWwindow* window;
};

class OffscreenWindow
{
public:
    struct Config
    {
        int context_version_major;
        int context_version_minor;
        int num_samples;
    };

    OffscreenWindow(const Config& config);
    ~OffscreenWindow();

private:
    GLFWwindow* window;
};

}  // namespace rb
