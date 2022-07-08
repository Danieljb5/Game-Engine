#include <log/include/log.hpp>
#include <assets/include/assets.hpp>
#include <game_object/include/game_object.hpp>
#include "../include/camera.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <map>
#include <list>

using namespace cl;
using namespace log;
using namespace assets;

SDL_Window* global_window = nullptr;
SDL_Renderer* global_renderer = nullptr;
std::map<int8_t, std::list<Sprite*>> draw_stack;
std::map<int8_t, std::list<TileMapBase*>> map_stack;

renderer::Camera* global_camera = nullptr;

extern "C"
{
    void init(LibraryManager* libManager)
    {
        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
        assets::detail::load_lib(libManager);
        log::detail::load_lib(libManager);
    }

    void create_window(const std::string& title, const unsigned int width, const unsigned int height, const uint32_t flags)
    {
        if(global_window)
        {
            log::error("cannot create window because it already exists");
            return;
        }

        if(global_renderer)
        {
            log::error("cannot create renderer because it already exists");
            return;
        }

        global_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        global_renderer = SDL_CreateRenderer(global_window, -1, SDL_RENDERER_ACCELERATED);
        assets::set_renderer(global_renderer);
        assets::set_window_surface(SDL_GetWindowSurface(global_window));
    }

    void create_fullscreen_window(const std::string& title, const uint32_t flags)
    {
        if(global_window)
        {
            log::error("cannot create window because it already exists");
            return;
        }

        if(global_renderer)
        {
            log::error("cannot create renderer because it already exists");
            return;
        }

        SDL_DisplayMode mode;
        SDL_GetDesktopDisplayMode(0, &mode);
        global_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.w, mode.h, flags | SDL_WINDOW_FULLSCREEN);
        global_renderer = SDL_CreateRenderer(global_window, -1, SDL_RENDERER_ACCELERATED);
        assets::set_renderer(global_renderer);
        assets::set_window_surface(SDL_GetWindowSurface(global_window));
    }

    void set_window_icon(const std::string& path)
    {
        SDL_Surface* surf = IMG_Load(path.c_str());
        SDL_SetWindowIcon(global_window, surf);
        SDL_FreeSurface(surf);
    }

    int get_refresh_rate()
    {
        SDL_DisplayMode mode;
        SDL_GetDesktopDisplayMode(SDL_GetWindowDisplayIndex(global_window), &mode);
        return mode.refresh_rate;
    }

    void destroy_window()
    {
        if(!global_window)
        {
            log::error("cannot destroy window because it does not exist");
            return;
        }

        if(!global_renderer)
        {
            log::error("cannot destroy renderer because it does not exist");
            return;
        }

        SDL_DestroyWindow(global_window);
        global_window = nullptr;
        SDL_DestroyRenderer(global_renderer);
        global_renderer = nullptr;
        assets::set_renderer(nullptr);
        assets::set_window_surface(nullptr);
    }

    void render_sprite(Sprite& spr)
    {
        auto _spr = (cl::assets::detail::Sprite*)&spr;
        if(!_spr->parent)
        {
            log::warn("tried to draw uninitialised sprite");
            return;
        }
        if(_spr->cache.dirty)
        {
            _spr->cache.tex_handle = _spr->parent->handle;
            const Vector2f pos = spr.get_position();
            const Vector2f scale = spr.get_scale();
            SDL_Rect rect;
            rect.x = pos.x;
            rect.y = pos.y;
            rect.w = scale.x * _spr->area.w;
            rect.h = scale.y * _spr->area.h;
            _spr->cache.dst_bounds = rect;
            _spr->cache.dirty = false;
        }
        auto area = _spr->area;
        auto dst = _spr->cache.dst_bounds;

        if(global_camera)
        {
            const auto offset = global_camera->GameObject().Transform().position;
            const auto zoom = global_camera->zoom;

            dst.x -= offset.x;
            dst.y -= offset.y;
            dst.x *= zoom;
            dst.y *= zoom;
            dst.w *= zoom;
            dst.h *= zoom;
        }

        if(SDL_RenderCopy(global_renderer, _spr->cache.tex_handle, &area, &dst) != 0)
        {
            log::error("failed to render sprite. Reason: "s + SDL_GetError());
        }
    }

    void render_tilemap(TileMapBase* map)
    {
        Vector2i startpos = (Vector2i)((global_camera->GameObject().Transform().position) / map->tile_size) - (Vector2i)1_v;
        Vector2i endpos = startpos + (Vector2i)((global_camera->GameObject().Transform().scale / map->tile_size) + (Vector2f)1_v);
        Vector2i pos = startpos;
        for(pos.x = startpos.x; pos.x <= endpos.x; pos.x++)
        {
            for(pos.y = startpos.y; pos.y <= endpos.y; pos.y++)
            {
                auto& tile = map->operator()(pos.x, pos.y);
                auto& spr = map->set[0];
                spr.set_position(pos * map->tile_size);
                auto& area = ((cl::assets::detail::Sprite*)&spr)->area;
                Vector2f os = {area.w, area.h};
                spr.set_scale((map->tile_size + (Vector2f)1_v) / os);
                render_sprite(spr);
            }
        }
    }

    void render_clear()
    {
        SDL_RenderClear(global_renderer);
    }

    void render_swap()
    {
        SDL_RenderPresent(global_renderer);
    }

    void push_render_stack(Sprite& spr)
    {
        draw_stack[spr.get_layer()].push_back(&spr);
    }

    void push_map_stack(int8_t layer, TileMapBase* map)
    {
        map_stack[layer].push_back(map);
    }

    void flush_render_stack()
    {
        render_clear();
        auto spr_it = draw_stack.begin();
        auto map_it = map_stack.begin();
        while(true)
        {
            if(spr_it != draw_stack.end())
            {
                if(spr_it->first <= map_it->first)
                {
                    for(auto& spr : spr_it->second)
                    {
                        render_sprite(*spr);
                    }
                    spr_it->second.clear();
                    spr_it++;
                }
            }
            if(map_it != map_stack.end())
            {
                if(map_it->first <= spr_it->first)
                {
                    for(auto map : map_it->second)
                    {
                        render_tilemap(map);
                    }
                    map_it++;
                }
            }
            if(spr_it == draw_stack.end() && map_it == map_stack.end()) break;
        }
        render_swap();
        draw_stack.clear();        
    }

    bool poll_event(SDL_Event& event)
    {
        if(event.type == SDL_WINDOWEVENT_RESIZED)
        {
            Vector2i size = 0_v;
            SDL_GetWindowSize(global_window, &size.x, &size.y);
            global_camera->GameObject().Transform().scale = size;
        }
        return SDL_PollEvent(&event);
    }

    SDL_Window* get_global_window()
    {
        return global_window;
    }

    SDL_Renderer* get_global_renderer()
    {
        return global_renderer;
    }

    void set_global_camera(renderer::Camera* cam)
    {
        global_camera = cam;
        Vector2i size = 0_v;
        SDL_GetWindowSize(global_window, &size.x, &size.y);
        cam->GameObject().Transform().scale = size;
    }
}