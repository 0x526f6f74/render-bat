#include "window.h"

#include <iostream>

#include "glad/glad.h"

namespace rb
{
    Window::Window(const WindowConfig& config) noexcept : config(config)
    { }

    Window::~Window() noexcept
    {
        glfwDestroyWindow(this->window);
        glfwTerminate();
    }

    void Window::initialize_glfw() const
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW!\n";
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->config.context_version.x);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->config.context_version.y);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, this->config.samples);
    }

    void Window::check_if_window_creation_was_successful() const
    {
        if (!this->window)
        {
            glfwTerminate();
            std::cerr << "Failed to create GLFW window!\n";
        }
    }

    void Window::make_opengl_context() const
    {
        glfwMakeContextCurrent(this->window);
        gladLoadGL();
    }

    RealtimeWindow::RealtimeWindow(const RealtimeWindowConfig& config) : Window(config.window_config), config(config)
    {
        this->initialize_glfw();

        this->window = glfwCreateWindow(this->config.size.x, this->config.size.y, this->config.title.c_str(), nullptr, nullptr);

        this->check_if_window_creation_was_successful();

        this->make_opengl_context();

        glfwSwapInterval(static_cast<int>(this->config.vsync));
        if (this->config.cursor_is_disabled) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetKeyCallback(window, this->config.key_callback);
        glfwSetMouseButtonCallback(window, this->config.mouse_button_callback);
    }

    glm::dvec2 RealtimeWindow::get_cursor_pos() const noexcept
    {
        glm::dvec2 cursor_pos;
        glfwGetCursorPos(this->window, &cursor_pos.x, &cursor_pos.y);
        return cursor_pos;
    }

    double RealtimeWindow::get_time() const noexcept
    {
        return glfwGetTime();
    }

    bool RealtimeWindow::should_close() const noexcept
    {
        return glfwWindowShouldClose(this->window);
    }

    void RealtimeWindow::update() const noexcept
    {
        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }

    int RealtimeWindow::get_key(int key) const noexcept
    {
        return glfwGetKey(this->window, key);
    }

    OffscreenWindow::OffscreenWindow(const OffscreenWindowConfig& config) : Window(config.window_config), config(config)
    {
        this->initialize_glfw();

        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        this->window = glfwCreateWindow(1, 1, "", nullptr, nullptr);

        this->check_if_window_creation_was_successful();

        this->make_opengl_context();
    }
}  // namespace rb
