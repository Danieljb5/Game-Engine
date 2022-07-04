#include <base/entry_setup.hpp>

#include <renderer/include/renderer.hpp>
#include <assets/include/assets.hpp>
#include <SDL.h>
#include <SDL_image.h>

int main()
{
    cl::renderer::create_window("Packing example", 1280, 720, SDL_WINDOW_SHOWN);

    auto atlases = cl::assets::generate_atlas("assets", {1024, 1024});
    auto a = atlases[0];

    auto s = a.get_sprite("assets/image.png"); // replace with a file name from assets

    bool running = true;
    while(running)
    {
        cl::thread::scoped_lazy_wait(1.0 / 60.0, 0.01);

        SDL_Event event;
        while(cl::renderer::poll_event(event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        cl::renderer::render_clear();
        cl::renderer::render_sprite(s);
        cl::renderer::render_swap();
    }

    cl::renderer::destroy_window();
    return 0;
}
