#include "state.h"

namespace rb
{
    void TimeState::update(const RealtimeWindow& window)
    {
        const auto current_frame_time = window.get_time();
        this->delta = current_frame_time - this->current;
        this->current = current_frame_time;
    }

    void KeyboardState::update(const RealtimeWindow& window)
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

    void MouseState::update(const RealtimeWindow& window)
    {
        const auto current_pos = window.get_cursor_pos();
        this->delta_pos = current_pos - this->cursor_pos;
        this->cursor_pos = current_pos;
    }

    State::State(const RealtimeWindow& window)
    {
        this->mouse.cursor_pos = window.get_cursor_pos();
        this->time.current = window.get_time();
    }

    void State::update(const RealtimeWindow& window)
    {
        this->time.update(window);
        this->keyboard.update(window);
        this->mouse.update(window);
    }
}  // namespace rb
