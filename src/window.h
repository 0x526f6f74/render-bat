#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <string>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace rb
{
    struct WindowConfig
    {
        glm::ivec2 context_version;
        int samples;
    };

    struct RealtimeWindow;

    struct RealtimeWindowConfig
    {
        WindowConfig window_config;

        std::string title;
        glm::ivec2 size;
        bool vsync;

        std::function<void(RealtimeWindow&, double, double)> scroll_callback;
        std::function<void(RealtimeWindow&, double, double)> cursor_pos_callback;
    };

    class Window
    {
    public:
        Window(const WindowConfig& config);
        virtual ~Window();

    protected:
        void initialize_glfw() const;
        void check_if_window_creation_was_successful() const;
        void make_opengl_context() const;

        GLFWwindow* window;

    private:
        const WindowConfig config;
    };

    struct RealtimeWindowState
    {
        const RealtimeWindowConfig config;

        double time;
        double dt;

        bool w_is_pressed = false;
        bool a_is_pressed = false;
        bool s_is_pressed = false;
        bool d_is_pressed = false;
        bool space_is_pressed = false;
        bool shift_is_pressed = false;

        glm::dvec2 cursor_pos;
        bool cursor_is_disabled = false;
        float zoom_level = 2.0f;
    };

    class RealtimeWindow : public Window
    {
    public:
        RealtimeWindow(const RealtimeWindowConfig& config);

        void update_time();
        void swap_buffers() const;

        const RealtimeWindowState& get_state() const;
        RealtimeWindowState& get_state();

        bool is_open() const;
        void close() const;

        void set_input_mode(int mode, int value) const;
        const glm::dvec2& get_cursor_pos() const;
        int get_key(int key) const;

    private:
        RealtimeWindowState state;
    };

    class OffscreenWindow : public Window
    {
    public:
        OffscreenWindow(const WindowConfig& config);
    };
}  // namespace rb
