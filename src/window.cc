#include "window.h"

#include "glad/glad.h"

namespace rb
{

RealtimeWindow::RealtimeWindow(const Config& config) : state({config})
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.context_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.context_version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, config.num_samples);

    this->window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);

    if (!this->window)
    {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window!\n";
    }

    glfwMakeContextCurrent(this->window);
    gladLoadGL();

    glfwSetWindowUserPointer(this->window, static_cast<void*>(this));
    glfwSwapInterval(static_cast<int>(config.vsync));

    glfwSetKeyCallback(
        this->window,
        [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            RealtimeWindow& self = *static_cast<RealtimeWindow*>(glfwGetWindowUserPointer(window));
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
                case GLFW_KEY_W: self.state.keyboard.w = action; break;
                case GLFW_KEY_A: self.state.keyboard.a = action; break;
                case GLFW_KEY_S: self.state.keyboard.s = action; break;
                case GLFW_KEY_D: self.state.keyboard.d = action; break;
                case GLFW_KEY_SPACE: self.state.keyboard.space = action; break;
                case GLFW_KEY_LEFT_SHIFT: self.state.keyboard.shift = action; break;
            }
        }
    );
    glfwSetMouseButtonCallback(
        this->window,
        [](GLFWwindow* window, int button, int action, int mods)
        {
            RealtimeWindow& self = *static_cast<RealtimeWindow*>(glfwGetWindowUserPointer(window));
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
            RealtimeWindow& self = *static_cast<RealtimeWindow*>(glfwGetWindowUserPointer(window));
            self.get_state().config.scroll_callback(self, xoffset, yoffset);
        }
    );
    glfwSetCursorPosCallback(
        this->window,
        [](GLFWwindow* window, double xpos, double ypos)
        {
            RealtimeWindow& self = *static_cast<RealtimeWindow*>(glfwGetWindowUserPointer(window));
            self.get_state().config.cursor_pos_callback(self, xpos, ypos);
        }
    );
}

RealtimeWindow::~RealtimeWindow()
{
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void RealtimeWindow::update()
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

RealtimeWindow::State& RealtimeWindow::get_state()
{
    return this->state;
}

const RealtimeWindow::State& RealtimeWindow::get_state() const
{
    return this->state;
}

OffscreenWindow::OffscreenWindow(const Config& config)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.context_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.context_version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, config.num_samples);
    glfwWindowHint(GLFW_VISIBLE, false);

    this->window = glfwCreateWindow(1, 1, "", nullptr, nullptr);

    if (!this->window)
    {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window!\n";
    }

    glfwMakeContextCurrent(this->window);
    gladLoadGL();
}

OffscreenWindow::~OffscreenWindow()
{
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

}  // namespace rb
