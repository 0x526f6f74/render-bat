#pragma once

#include "window.h"

namespace rb
{
    struct TimeState
    {
        double current;
        double delta;

        void update(const RealtimeWindow& window) noexcept;
    };

    struct KeyboardState
    {
        bool w_is_pressed = false;
        bool a_is_pressed = false;
        bool s_is_pressed = false;
        bool d_is_pressed = false;
        bool space_is_pressed = false;
        bool shift_is_pressed = false;

        void update(const RealtimeWindow& window) noexcept;
    };

    struct MouseState
    {
        glm::dvec2 cursor_pos;
        glm::dvec2 delta_pos;

        void update(const RealtimeWindow& window) noexcept;
    };

    struct State
    {
        State(const RealtimeWindow& window) noexcept;

        void update(const RealtimeWindow& window) noexcept;

        TimeState time;
        KeyboardState keyboard;
        MouseState mouse;
    };
}  // namespace rb