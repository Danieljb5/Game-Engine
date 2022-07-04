#pragma once

#include <base/library_loader.hpp>
#include <assets/include/atlas.hpp>
#include <SDL.h>

namespace cl
{
    namespace renderer
    {
        LIBRARY(cl_renderer)
        
        STUB_A(create_window, void, const std::string& title, const uint width, const uint height, const uint32_t flags)
        return create_window(title, width, height, flags);
        STUB_B(create_window, void, const std::string& title, const uint width, const uint height, const uint32_t flags)

        STUB(destroy_window, void)

        STUB_A(render_sprite, void, const assets::Sprite& spr)
        return render_sprite(spr);
        STUB_B(render_sprite, void, const assets::Sprite& spr)

        STUB(render_clear, void)

        STUB(render_swap, void)

        STUB_A(poll_event, bool, SDL_Event& event)
        return poll_event(event);
        STUB_B_RET(poll_event, bool, SDL_Event& event)

        STUB_RET_PTR(get_global_window, SDL_Window*)

        STUB_RET_PTR(get_global_renderer, SDL_Renderer*)
    }
}