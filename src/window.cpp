#include "window.h"

#include <iostream>

#include "glad/glad.h"

namespace rb
{
    Window::Window(const WindowConfig& config) : config(config)
    { }

    Window::~Window()
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
        if (this->state.cursor_is_disabled) glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetKeyCallback(this->window, this->config.key_callback);
        glfwSetMouseButtonCallback(this->window, this->config.mouse_button_callback);
        glfwSetScrollCallback(this->window, this->config.scroll_callback);
        glfwSetWindowUserPointer(this->window, static_cast<void*>(&this->state));
    }

    bool RealtimeWindow::is_open() const
    {
        return !glfwWindowShouldClose(this->window);
    }

    const RealtimeWindowState& RealtimeWindow::get_state() const
    {
        return this->state;
    }

    glm::dvec2 RealtimeWindow::get_cursor_pos() const
    {
        glm::dvec2 cursor_pos;
        glfwGetCursorPos(this->window, &cursor_pos.x, &cursor_pos.y);
        return cursor_pos;
    }

    int RealtimeWindow::get_key(int key) const
    {
        return glfwGetKey(this->window, key);
    }

    void RealtimeWindow::update()
    {
        const auto current_time = glfwGetTime();
        this->state.dt = current_time - this->state.time;
        this->state.time = current_time;

        switch (glfwGetKey(this->window, GLFW_KEY_W))
        {
            case GLFW_PRESS: this->state.w_is_pressed = true; break;
            case GLFW_RELEASE: this->state.w_is_pressed = false; break;
        }
        switch (glfwGetKey(this->window, GLFW_KEY_A))
        {
            case GLFW_PRESS: this->state.a_is_pressed = true; break;
            case GLFW_RELEASE: this->state.a_is_pressed = false; break;
        }
        switch (glfwGetKey(this->window, GLFW_KEY_S))
        {
            case GLFW_PRESS: this->state.s_is_pressed = true; break;
            case GLFW_RELEASE: this->state.s_is_pressed = false; break;
        }
        switch (glfwGetKey(this->window, GLFW_KEY_D))
        {
            case GLFW_PRESS: this->state.d_is_pressed = true; break;
            case GLFW_RELEASE: this->state.d_is_pressed = false; break;
        }
        switch (glfwGetKey(this->window, GLFW_KEY_SPACE))
        {
            case GLFW_PRESS: this->state.space_is_pressed = true; break;
            case GLFW_RELEASE: this->state.space_is_pressed = false; break;
        }
        switch (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT))
        {
            case GLFW_PRESS: this->state.shift_is_pressed = true; break;
            case GLFW_RELEASE: this->state.shift_is_pressed = false; break;
        }

        const auto current_cursor_pos = this->get_cursor_pos();
        this->state.delta_pos = current_cursor_pos - this->state.cursor_pos;
        this->state.cursor_pos = current_cursor_pos;
    }

    void RealtimeWindow::swap_buffers() const
    {
        glfwSwapBuffers(this->window);
        glfwPollEvents();
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
