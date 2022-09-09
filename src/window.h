#pragma once

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

    struct RealtimeWindowConfig
    {
        WindowConfig window_config;

        std::string title;
        glm::ivec2 size;
        bool vsync;
        GLFWkeyfun key_callback;
        GLFWmousebuttonfun mouse_button_callback;
        GLFWscrollfun scroll_callback;
    };

    struct OffscreenWindowConfig
    {
        const WindowConfig window_config;
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
        double time;
        double dt;

        bool w_is_pressed = false;
        bool a_is_pressed = false;
        bool s_is_pressed = false;
        bool d_is_pressed = false;
        bool space_is_pressed = false;
        bool shift_is_pressed = false;

        bool cursor_is_disabled = false;

        glm::dvec2 cursor_pos;
        glm::dvec2 delta_pos;
    };

    class RealtimeWindow : public Window
    {
    public:
        RealtimeWindow(const RealtimeWindowConfig& config);

        bool is_open() const;
        glm::dvec2 get_cursor_pos() const;
        int get_key(int key) const;

        const RealtimeWindowState& get_state() const;

        void update();
        void swap_buffers() const;

    private:
        const RealtimeWindowConfig config;
        RealtimeWindowState state;
    };

    class OffscreenWindow : public Window
    {
    public:
        OffscreenWindow(const OffscreenWindowConfig& config);

    private:
        const OffscreenWindowConfig config;
    };
}  // namespace rb
