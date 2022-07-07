#pragma once

#include <base/library_loader.hpp>
#include <assets/include/atlas.hpp>
#include <SDL.h>

namespace cl
{
    namespace renderer
    {
        LIBRARY(cl_renderer)
        
        STUB_A(create_window, void, const std::string& title, const unsigned int width, const unsigned int height, const uint32_t flags)
        return create_window(title, width, height, flags);
        STUB_B(create_window, void, const std::string& title, const unsigned int width, const unsigned int height, const uint32_t flags)

        STUB_A(create_fullscreen_window, void, const std::string& title, const uint32_t flags)
        return create_fullscreen_window(title, flags);
        STUB_B(create_fullscreen_window, void, const std::string& title, const uint32_t flags)

        STUB_A(set_window_icon, void, const std::string& path)
        return set_window_icon(path);
        STUB_B(set_window_icon, void, const std::string& path)

        STUB_A(get_refresh_rate, int)
        return get_refresh_rate();
        STUB_B_RET(get_refresh_rate, int)

        STUB(destroy_window, void)

        STUB_A(push_render_stack, void, assets::Sprite& spr)
        return push_render_stack(spr);
        STUB_B(push_render_stack, void, assets::Sprite& spr)

        STUB(flush_render_stack, void);

        // render_sprite, render_clear and render_swap can all be replaced by calling push_render_stack and flush_render_stack

        STUB_A(render_sprite, void, const assets::Sprite& spr)
        return render_sprite(spr);
        STUB_B(render_sprite, void, const assets::Sprite& spr)

        STUB(render_clear, void)

        STUB(render_swap, void)

        //

        STUB_A(poll_event, bool, SDL_Event& event)
        return poll_event(event);
        STUB_B_RET(poll_event, bool, SDL_Event& event)

        STUB_RET_PTR(get_global_window, SDL_Window*)

        STUB_RET_PTR(get_global_renderer, SDL_Renderer*)
    }
}

#include "renderComponents.hpp"