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

    RealtimeWindow::RealtimeWindow(const RealtimeWindowConfig& config) : Window(config.window_config), state({config})
    {
        this->initialize_glfw();

        this->window = glfwCreateWindow(this->state.config.size.x, this->state.config.size.y, this->state.config.title.c_str(), nullptr, nullptr);

        this->check_if_window_creation_was_successful();

        this->make_opengl_context();

        glfwSetWindowUserPointer(this->window, reinterpret_cast<void*>(this));
        glfwSwapInterval(static_cast<int>(this->state.config.vsync));
        if (this->state.cursor_is_disabled) glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetKeyCallback(
            this->window,
            [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                RealtimeWindow& self = *reinterpret_cast<RealtimeWindow*>(glfwGetWindowUserPointer(window));
                auto& state = self.get_state();
                if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && state.cursor_is_disabled)
                {
                    state.cursor_is_disabled = false;
                    self.set_input_mode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
                else if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
                    self.close();
                switch (key)
                {
                    case GLFW_KEY_W: self.state.w_is_pressed = action; break;
                    case GLFW_KEY_A: self.state.a_is_pressed = action; break;
                    case GLFW_KEY_S: self.state.s_is_pressed = action; break;
                    case GLFW_KEY_D: self.state.d_is_pressed = action; break;
                    case GLFW_KEY_LEFT_SHIFT: self.state.shift_is_pressed = action; break;
                    case GLFW_KEY_SPACE: self.state.space_is_pressed = action; break;
                }
            }
        );
        glfwSetMouseButtonCallback(
            this->window,
            [](GLFWwindow* window, int button, int action, int mods)
            {
                RealtimeWindow& self = *reinterpret_cast<RealtimeWindow*>(glfwGetWindowUserPointer(window));
                auto& state = self.get_state();
                if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !state.cursor_is_disabled)
                {
                    state.cursor_is_disabled = true;
                    self.set_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }
            }
        );
        glfwSetScrollCallback(
            this->window,
            [](GLFWwindow* window, double xoffset, double yoffset)
            {
                RealtimeWindow& self = *reinterpret_cast<RealtimeWindow*>(glfwGetWindowUserPointer(window));
                self.get_state().config.scroll_callback(self, xoffset, yoffset);
            }
        );
        glfwSetCursorPosCallback(
            this->window,
            [](GLFWwindow* window, double xpos, double ypos)
            {
                RealtimeWindow& self = *reinterpret_cast<RealtimeWindow*>(glfwGetWindowUserPointer(window));
                self.get_state().config.cursor_pos_callback(self, xpos, ypos);
            }
        );
    }

    void RealtimeWindow::update_time()
    {
        const auto current_time = glfwGetTime();
        this->state.dt = current_time - this->state.time;
        this->state.time = current_time;
    }

    void RealtimeWindow::swap_buffers() const
    {
        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }

    const RealtimeWindowState& RealtimeWindow::get_state() const
    {
        return this->state;
    }

    RealtimeWindowState& RealtimeWindow::get_state()
    {
        return this->state;
    }

    bool RealtimeWindow::is_open() const
    {
        return !glfwWindowShouldClose(this->window);
    }

    void RealtimeWindow::close() const
    {
        glfwSetWindowShouldClose(this->window, true);
    }

    void RealtimeWindow::set_input_mode(int mode, int value) const
    {
        glfwSetInputMode(this->window, mode, value);
    }

    const glm::dvec2& RealtimeWindow::get_cursor_pos() const
    {
        return this->state.cursor_pos;
    }

    int RealtimeWindow::get_key(int key) const
    {
        return glfwGetKey(this->window, key);
    }

    OffscreenWindow::OffscreenWindow(const WindowConfig& config) : Window(config)
    {
        this->initialize_glfw();

        glfwWindowHint(GLFW_VISIBLE, false);

        this->window = glfwCreateWindow(1, 1, "", nullptr, nullptr);

        this->check_if_window_creation_was_successful();

        this->make_opengl_context();
    }
}  // namespace rb
