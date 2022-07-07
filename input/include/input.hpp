#pragma once

#include <base/library_loader.hpp>
#include "keycodes.hpp"
#include "events.hpp"
#include <vector2/include/vector2.hpp>

namespace cl
{
    namespace input
    {
        LIBRARY(cl_input)

        STUB_A(push_event, void, SDL_Event* event)
        return push_event(event);
        STUB_B(push_event, void, SDL_Event* event)

        STUB_A(subscribe_keydown_event, void, void (*func)(KeyCode, Keymod))
        return subscribe_keydown_event(func);
        STUB_B(subscribe_keydown_event, void, void (*func)(KeyCode, Keymod))

        STUB_A(subscribe_keyup_event, void, void (*func)(KeyCode, Keymod))
        return subscribe_keyup_event(func);
        STUB_B(subscribe_keyup_event, void, void (*func)(KeyCode, Keymod))

        STUB_A(subscribe_key_event, void, void (*func)(KeyCode, Keymod))
        return subscribe_key_event(func);
        STUB_B(subscribe_key_event, void, void (*func)(KeyCode, Keymod))

        STUB_A(subscribe_mousebuttondown_event, void, void (*func)(uint8_t))
        return subscribe_mousebuttondown_event(func);
        STUB_B(subscribe_mousebuttondown_event, void, void (*func)(uint8_t))

        STUB_A(subscribe_mousebuttonup_event, void, void (*func)(uint8_t))
        return subscribe_mousebuttonup_event(func);
        STUB_B(subscribe_mousebuttonup_event, void, void (*func)(uint8_t))

        STUB_A(subscribe_mousebutton_event, void, void (*func)(uint8_t))
        return subscribe_mousebutton_event(func);
        STUB_B(subscribe_mousebutton_event, void, void (*func)(uint8_t))

        STUB_A(subscribe_mousewheel_event, void, void (*func)(cl::Vector2i))
        return subscribe_mousewheel_event(func);
        STUB_B(subscribe_mousewheel_event, void, void (*func)(cl::Vector2i))

        STUB_A(subscribe_mousemotion_event, void, void (*func)(cl::Vector2i))
        return subscribe_mousemotion_event(func);
        STUB_B(subscribe_mousemotion_event, void, void (*func)(cl::Vector2i))

        STUB(manage_subscriptions, void)
    }
}