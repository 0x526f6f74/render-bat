#pragma once

#include "window.h"

namespace rb
{
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

    struct State
    {
        void update(const RealtimeWindow& window, double dt) noexcept;

        KeyboardState keyboard;

        glm::dvec2 cursor_pos;
        double previous_frame_time;
    };
}  // namespace rb
