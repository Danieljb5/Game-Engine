#include <base/entry_setup.hpp>

#include <renderer/include/renderer.hpp>
#include <assets/include/assets.hpp>

int main(int argc, char** argv)
{
    cl::renderer::create_window("Packing example", 1280, 720, SDL_WINDOW_SHOWN);

    auto a = cl::assets::generate_atlas("assets", {1024, 1024});

    auto s = a.get_sprite("assets/image.png");

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

        cl::renderer::push_render_stack(s);
        cl::renderer::flush_render_stack();

        // these are replaced by push_render_stack, which pushes sprites into a draw queue and flush_render_stack, which clears the screen, draws objects in the queue and swaps the buffers
        // cl::renderer::render_clear();
        // cl::renderer::render_sprite(s);
        // cl::renderer::render_swap();
    }

    cl::renderer::destroy_window();
    return 0;
}
