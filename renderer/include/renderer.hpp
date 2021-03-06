#pragma once

#include <base/library_loader.hpp>
#include <assets/include/atlas.hpp>
#include "camera.hpp"
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

        STUB_A(push_map_stack, void, int8_t layer, TileMapBase* map)
        return push_map_stack(layer, map);
        STUB_B(push_map_stack, void, int8_t layer, TileMapBase* map)

        STUB(flush_render_stack, void);

        // render_sprite, render_clear, etc. can all be replaced by calling push_render_stack, push_map_stack and flush_render_stack

        STUB_A(render_sprite, void, const assets::Sprite& spr)
        return render_sprite(spr);
        STUB_B(render_sprite, void, const assets::Sprite& spr)

        STUB_A(render_tilemap, void, TileMapBase* map)
        return render_tilemap(map);
        STUB_B(render_tilemap, void, TileMapBase* map)

        STUB(render_clear, void)

        STUB(render_swap, void)

        //

        STUB_A(poll_event, bool, SDL_Event& event)
        return poll_event(event);
        STUB_B_RET(poll_event, bool, SDL_Event& event)

        STUB_RET_PTR(get_global_window, SDL_Window*)
        
        STUB_RET_PTR(get_global_renderer, SDL_Renderer*)

        STUB_A(set_global_camera, void, cl::renderer::Camera* cam)
        return set_global_camera(cam);
        STUB_B(set_global_camera, void, cl::renderer::Camera* cam)
    }
}

#include "renderComponents.hpp"