#include <base/entry_setup.hpp>
#include <game_object/include/game_object.hpp>
#include <log/include/log.hpp>
#include <vector2/include/vector2.hpp>
#include <renderer/include/renderer.hpp>

int main(int argc, char** argv)
{
    cl::renderer::create_window("Renderer test", 1280, 720, SDL_WINDOW_SHOWN);

    auto atlas = cl::assets::generate_atlas("assets", 1024_v);

    cl::GameObject go;
    go.AddComponent(new cl::renderer::SpriteRenderer());
    go.GetComponent<cl::renderer::SpriteRenderer>().Sprite = atlas.get_sprite("assets/image.png");
    cl::GameObject go2;
    go2.AddComponent(new cl::renderer::SpriteRenderer());
    go2.GetComponent<cl::renderer::SpriteRenderer>().Sprite = atlas.get_sprite("assets/image2.png");
    go2.GetComponent<cl::renderer::SpriteRenderer>().Sprite.set_layer(1);

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
        ((cl::detail::GameObject*)&go2)->_update();

        go.Transform().position.x = 100;

        ((cl::detail::GameObject*)&go)->_late_update();
        ((cl::detail::GameObject*)&go2)->_late_update();
        ((cl::detail::GameObject*)&go)->_on_draw();
        ((cl::detail::GameObject*)&go2)->_on_draw();

        cl::renderer::flush_render_stack();
    }

    cl::renderer::destroy_window();
    return 0;
}
