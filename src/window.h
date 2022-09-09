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
        bool cursor_is_disabled;
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

    class RealtimeWindow : public Window
    {
    public:
        RealtimeWindow(const RealtimeWindowConfig& config);

        bool is_open() const;
        glm::dvec2 get_cursor_pos() const;
        int get_key(int key) const;

        double get_time() const;
        double get_dt() const;

        void update();
        void swap_buffers() const;

    private:
        const RealtimeWindowConfig config;

        double time = 0.0;
        double dt = 0.01;
    };

    class OffscreenWindow : public Window
    {
    public:
        OffscreenWindow(const OffscreenWindowConfig& config);

    private:
        const OffscreenWindowConfig config;
    };
}  // namespace rb
