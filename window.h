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
    };

    struct OffscreenWindowConfig
    {
        const WindowConfig window_config;
    };

    class Window
    {
    public:
        Window(const WindowConfig& config) noexcept;
        virtual ~Window() noexcept;

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

        glm::dvec2 get_cursor_pos() const noexcept;
        double get_time() const noexcept;
        bool should_close() const noexcept;
        void update() const noexcept;
        int get_key(int key) const noexcept;

    private:
        const RealtimeWindowConfig config;
    };

    class OffscreenWindow : public Window
    {
    public:
        OffscreenWindow(const OffscreenWindowConfig& config);

    private:
        const OffscreenWindowConfig config;
    };
}  // namespace rb
