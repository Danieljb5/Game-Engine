#include <log/include/log.hpp>
#include <assets/include/assets.hpp>
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
        if(SDL_RenderCopy(global_renderer, _spr->cache.tex_handle, &area, &dst) != 0)
        {
            log::error("failed to render sprite. Reason: "s + SDL_GetError());
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

    void flush_render_stack()
    {
        render_clear();
        for(auto it = draw_stack.begin(); it != draw_stack.end(); it++)
        {
            for(auto& spr : it->second)
            {
                render_sprite(*spr);
            }
            it->second.clear();
        }
        render_swap();
        draw_stack.clear();        
    }

    bool poll_event(SDL_Event& event)
    {
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
}
