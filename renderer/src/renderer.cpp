#include <log/include/log.hpp>
#include <assets/include/assets.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

using namespace cl;
using namespace log;
using namespace assets;

SDL_Window* global_window = nullptr;
SDL_Renderer* global_renderer = nullptr;

extern "C"
{
    void init(LibraryManager* libManager)
    {
        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
        assets::detail::load_lib(libManager);
        log::detail::load_lib(libManager);
    }

    void create_window(const std::string& title, const uint width, const uint height, const uint32_t flags)
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

    void render_sprite(const Sprite& spr)
    {
        cl::assets::detail::Sprite _spr = *(cl::assets::detail::Sprite*)&spr;
        if(_spr.cache.dirty)
        {
            _spr.cache.tex_handle = _spr.parent->handle;
            Vector2f pos = spr.get_position();
            Vector2f scale = spr.get_scale();
            SDL_Rect rect;
            rect.x = pos.x;
            rect.y = pos.y;
            rect.w = scale.x * _spr.area.w;
            rect.h = scale.y * _spr.area.h;
            _spr.cache.dst_bounds = rect;
            _spr.cache.dirty = false;
        }
        SDL_RenderCopy(global_renderer, _spr.cache.tex_handle, &_spr.area, &_spr.cache.dst_bounds);
    }

    void render_clear()
    {
        SDL_RenderClear(global_renderer);
    }

    void render_swap()
    {
        SDL_RenderPresent(global_renderer);
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