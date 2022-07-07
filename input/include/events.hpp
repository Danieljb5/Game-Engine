#pragma once

#include <SDL.h>

namespace cl
{
    namespace input
    {
        enum Event
        {
            KeyDown,
            KeyUp,
            Key,
            MouseButtonDown,
            MouseButtonUp,
            MouseButton,
            MouseWheel,
            MouseMotion,
            EVENT_COUNT
        };
    }
}