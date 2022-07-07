#include <base/library_loader.hpp>
#include <vector2/include/vector2.hpp>
#include "../include/keycodes.hpp"
#include "../include/events.hpp"
#include <SDL.h>
#include <vector>
#include <unordered_map>

using namespace cl;
using namespace input;

std::unordered_map<KeyCode, Keymod> active_keys;
std::unordered_map<KeyCode, Keymod> pressed_keys;
std::unordered_map<KeyCode, Keymod> released_keys;
std::unordered_map<uint8_t, bool> active_buttons;
std::unordered_map<uint8_t, bool> pressed_buttons;
std::unordered_map<uint8_t, bool> released_buttons;
Vector2i mouse_pos = 0_v;
Vector2i mouse_wheel = 0_v;

bool event_state[EVENT_COUNT] = { false };

std::vector<void (*)(KeyCode, Keymod)> keydown_subscriptions;
std::vector<void (*)(KeyCode, Keymod)> keyup_subscriptions;
std::vector<void (*)(KeyCode, Keymod)> key_subscriptions;
std::vector<void (*)(uint8_t)> mousebuttondown_subscriptions;
std::vector<void (*)(uint8_t)> mousebuttonup_subscriptions;
std::vector<void (*)(uint8_t)> mousebutton_subscriptions;
std::vector<void (*)(Vector2i)> mousewheel_subscriptions;
std::vector<void (*)(Vector2i)> mousemotion_subscriptions;

extern "C"
{
    void init(LibraryManager* libManager)
    {

    }

    void push_event(SDL_Event* event)
    {
        switch(event->type)
        {
        case SDL_KEYDOWN:
            if(!active_keys.count((KeyCode)event->key.keysym.sym))
            {
                active_keys[(KeyCode)event->key.keysym.sym] = (Keymod)event->key.keysym.mod;
                pressed_keys[(KeyCode)event->key.keysym.sym] = (Keymod)event->key.keysym.mod;
                event_state[KeyDown] = true;
            }
            break;

        case SDL_KEYUP:
            if(active_keys.count((KeyCode)event->key.keysym.sym)) active_keys.erase((KeyCode)event->key.keysym.sym);
            released_keys[(KeyCode)event->key.keysym.sym] = (Keymod)event->key.keysym.mod;
            event_state[KeyUp] = true;
            break;

        case SDL_MOUSEBUTTONDOWN:
            active_buttons[event->button.button] = true;
            pressed_buttons[event->button.button] = true;
            event_state[MouseButtonDown] = true;
            break;

        case SDL_MOUSEBUTTONUP:
            if(active_buttons.count(event->button.button)) active_buttons.erase(event->button.button);
            released_buttons[event->button.button] = true;
            event_state[MouseButtonUp] = true;
            break;

        case SDL_MOUSEMOTION:
            mouse_pos.x = event->motion.x;
            mouse_pos.y = event->motion.y;
            event_state[MouseMotion] = true;
            break;

        case SDL_MOUSEWHEEL:
            mouse_wheel.x = event->wheel.x;
            mouse_wheel.y = event->wheel.y;
            event_state[MouseWheel] = true;
            break;

        default:
            break;
        }
    }

    void subscribe_keydown_event(void (*func)(KeyCode, Keymod))
    {
        keydown_subscriptions.push_back(func);
    }

    void subscribe_keyup_event(void (*func)(KeyCode, Keymod))
    {
        keyup_subscriptions.push_back(func);
    }

    void subscribe_key_event(void (*func)(KeyCode, Keymod))
    {
        key_subscriptions.push_back(func);
    }

    void subscribe_mousebuttondown_event(void (*func)(uint8_t))
    {
        mousebuttondown_subscriptions.push_back(func);
    }

    void subscribe_mousebuttonup_event(void (*func)(uint8_t))
    {
        mousebuttonup_subscriptions.push_back(func);
    }

    void subscribe_mousebutton_event(void (*func)(uint8_t))
    {
        mousebutton_subscriptions.push_back(func);
    }

    void subscribe_mousewheel_event(void (*func)(cl::Vector2i))
    {
        mousewheel_subscriptions.push_back(func);
    }

    void subscribe_mousemotion_event(void (*func)(cl::Vector2i))
    {
        mousemotion_subscriptions.push_back(func);
    }

    void manage_subscriptions()
    {
        if(event_state[KeyDown])
        {
            for(auto it = pressed_keys.begin(); it != pressed_keys.end(); it++)
            {
                for(auto& func : keydown_subscriptions)
                {
                    func(it->first, it->second);
                }
            }
        }
        event_state[KeyDown] = false;
        pressed_keys.clear();

        if(event_state[KeyUp])
        {
            for(auto it = released_keys.begin(); it != released_keys.end(); it++)
            {
                for(auto& func : keyup_subscriptions)
                {
                    func(it->first, it->second);
                }
            }
        }
        event_state[KeyUp] = false;
        released_keys.clear();

        for(auto it = active_keys.begin(); it != active_keys.end(); it++)
        {
            for(auto& func : key_subscriptions)
            {
                func(it->first, it->second);
            }
        }

        if(event_state[MouseButtonDown])
        {
            for(auto it = pressed_buttons.begin(); it != pressed_buttons.end(); it++)
            {
                for(auto& func : mousebuttondown_subscriptions)
                {
                    func(it->first);
                }
            }
        }
        event_state[MouseButtonDown] = false;
        pressed_buttons.clear();

        if(event_state[MouseButtonUp])
        {
            for(auto it = released_buttons.begin(); it != released_buttons.end(); it++)
            {
                for(auto& func : mousebuttonup_subscriptions)
                {
                    func(it->first);
                }
            }
        }
        event_state[MouseButtonUp] = false;
        released_buttons.clear();

        for(auto it = active_buttons.begin(); it != active_buttons.end(); it++)
        {
            for(auto& func : mousebutton_subscriptions)
            {
                func(it->first);
            }
        }

        if(event_state[MouseMotion])
        {
            for(auto& func : mousemotion_subscriptions)
            {
                func(mouse_pos);
            }
        }

        if(event_state[MouseWheel])
        {
            for(auto& func : mousewheel_subscriptions)
            {
                func(mouse_pos);
            }
        }
    }
}