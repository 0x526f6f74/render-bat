#include "state.h"

namespace rb
{
    void KeyboardState::update(const RealtimeWindow& window) noexcept
    {
        switch (window.get_key(GLFW_KEY_W))
        {
            case GLFW_PRESS: this->w_is_pressed = true; break;
            case GLFW_RELEASE: this->w_is_pressed = false; break;
        }
        switch (window.get_key(GLFW_KEY_A))
        {
            case GLFW_PRESS: this->a_is_pressed = true; break;
            case GLFW_RELEASE: this->a_is_pressed = false; break;
        }
        switch (window.get_key(GLFW_KEY_S))
        {
            case GLFW_PRESS: this->s_is_pressed = true; break;
            case GLFW_RELEASE: this->s_is_pressed = false; break;
        }
        switch (window.get_key(GLFW_KEY_D))
        {
            case GLFW_PRESS: this->d_is_pressed = true; break;
            case GLFW_RELEASE: this->d_is_pressed = false; break;
        }
        switch (window.get_key(GLFW_KEY_SPACE))
        {
            case GLFW_PRESS: this->space_is_pressed = true; break;
            case GLFW_RELEASE: this->space_is_pressed = false; break;
        }
        switch (window.get_key(GLFW_KEY_LEFT_SHIFT))
        {
            case GLFW_PRESS: this->shift_is_pressed = true; break;
            case GLFW_RELEASE: this->shift_is_pressed = false; break;
        }
    }

    void MouseState::update(const RealtimeWindow& window) noexcept
    {
        const auto current_pos = window.get_cursor_pos();
        this->delta_pos = current_pos - this->cursor_pos;
        this->cursor_pos = current_pos;
    }

    void State::update(const RealtimeWindow& window, double dt) noexcept
    {
        this->keyboard.update(window);
        this->mouse.update(window);
    }
}  // namespace rb
