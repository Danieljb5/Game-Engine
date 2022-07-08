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
    go.GetComponent<cl::renderer::SpriteRenderer>().Sprite.set_layer(1);
    cl::GameObject go2;
    go2.AddComponent(new cl::SparseTileMap<32>());
    go2.AddComponent(new cl::renderer::TileMapRenderer());
    auto& map = go2.GetComponent<cl::SparseTileMap<32>>();
    auto spr = atlas.get_sprite("assets/image2.png");
    map.tile_size = 32_v/32_v;
    map.set.add_sprite(0, spr);
    cl::GameObject camera;
    camera.AddComponent(new cl::renderer::Camera());
    camera.GetComponent<cl::renderer::Camera>().SetAsMainCamera();

    ((cl::detail::GameObject*)&go)->_start();
    ((cl::detail::GameObject*)&go2)->_start();
    ((cl::detail::GameObject*)&camera)->_start();

    float dt = 0;
    cl::thread::calculate_delta_time();

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
        ((cl::detail::GameObject*)&camera)->_update();

        go.Transform().position.x = 100;
        camera.Transform().position.x -= 10 * dt;
        camera.GetComponent<cl::renderer::Camera>().zoom += 0.05 * dt;

        ((cl::detail::GameObject*)&go)->_late_update();
        ((cl::detail::GameObject*)&go2)->_late_update();
        ((cl::detail::GameObject*)&camera)->_late_update();
        ((cl::detail::GameObject*)&go)->_on_draw();
        ((cl::detail::GameObject*)&go2)->_on_draw();
        ((cl::detail::GameObject*)&camera)->_on_draw();

        cl::renderer::flush_render_stack();

        cl::thread::calculate_delta_time();
        dt = cl::thread::delta_time();
    }

    cl::renderer::destroy_window();
    return 0;
}
