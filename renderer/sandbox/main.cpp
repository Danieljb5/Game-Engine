#include <base/entry_setup.hpp>
#include <game_object/include/game_object.hpp>
#include <log/include/log.hpp>
#include <vector2/include/vector2.hpp>
#include <renderer/include/renderer.hpp>

int main()
{
    cl::renderer::create_window("Renderer test", 1280, 720, SDL_WINDOW_SHOWN);

    auto atlases = cl::assets::generate_atlas("assets", 1024_v);
    auto atlas = atlases[0];

    cl::GameObject go;
    go.AddComponent(new cl::renderer::SpriteRenderer());
    go.GetComponent<cl::renderer::SpriteRenderer>().Sprite = atlas.get_sprite("assets/image.png");

    ((cl::detail::GameObject*)&go)->_start();

    bool running = true;
    while(running)
    {
        SDL_Event event;
        while(cl::renderer::poll_event(event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        ((cl::detail::GameObject*)&go)->_update();

        go.Transform().position.x = 100;

        ((cl::detail::GameObject*)&go)->_late_update();
        ((cl::detail::GameObject*)&go)->_on_draw();

        cl::renderer::flush_render_stack();
    }

    cl::renderer::destroy_window();
    return 0;
}