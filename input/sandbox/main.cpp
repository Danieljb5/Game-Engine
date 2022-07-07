#include <base/entry_setup.hpp>
#include <renderer/include/renderer.hpp>
#include <input/include/input.hpp>
#include <log/include/log.hpp>

void keydown(cl::input::KeyCode key, cl::input::Keymod mod)
{
    cl::log::info("Key pressed: "s + std::to_string(key) + " '"s + (char)key + "'"s);
    cl::log::info("Key modifiers: "s + std::to_string(mod));
}

void keyup(cl::input::KeyCode key, cl::input::Keymod mod)
{
    cl::log::info("Key released: "s + std::to_string(key) + " '"s + (char)key + "'"s);
    cl::log::info("Key modifiers: "s + std::to_string(mod));
}

void buttondown(uint8_t button)
{
    cl::log::info("Button pressed: "s + std::to_string((int)button));
}

void buttonup(uint8_t button)
{
    cl::log::info("Button released: "s + std::to_string((int)button));
}

int main(int argc, char** argv)
{
    cl::renderer::create_window("Input test", 1280, 720, SDL_WINDOW_SHOWN);

    cl::input::subscribe_keydown_event(keydown);
    cl::input::subscribe_keyup_event(keyup);
    cl::input::subscribe_mousebuttondown_event(buttondown);
    cl::input::subscribe_mousebuttonup_event(buttonup);

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
            cl::input::push_event(&event);
        }
        cl::input::manage_subscriptions();

        cl::renderer::flush_render_stack();
    }

    cl::renderer::destroy_window();
    return 0;
}