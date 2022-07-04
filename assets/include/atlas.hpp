#pragma once

#include <vector2/include/vector2.hpp>
#include <SDL.h>
#include <string>
#include <unordered_map>

namespace cl
{
    namespace assets
    {
        namespace detail
        {
            struct Atlas;

            struct CacheObject
            {
                CacheObject()
                {
                    dirty = true;
                }

                bool dirty = true;
                SDL_Rect dst_bounds;
                SDL_Texture* tex_handle = nullptr;
            };

            struct Sprite
            {
                Sprite(Atlas* parent, const SDL_Rect& area)
                {
                    this->parent = parent;
                    this->area = area;
                }

                Atlas* parent;
                SDL_Rect area;
                CacheObject cache;
            };

            struct Atlas
            {
                SDL_Texture* handle;
                std::unordered_map<std::string, SDL_Rect> path_map;
            };
        }
        
        struct Sprite : public detail::Sprite
        {
            Sprite(detail::Atlas* parent, const SDL_Rect& area) : detail::Sprite(parent, area)
            {

            }

            void set_position(const Vector2f& position)
            {
                this->position = position;
                cache.dirty = true;
            }

            void set_scale(const Vector2f& scale)
            {
                this->position = scale;
                cache.dirty = true;
            }

            const Vector2f& get_position() const
            {
                return this->position;
            }

            const Vector2f& get_scale() const
            {
                return this->scale;
            }

        private:
            Vector2f position = {0, 0};
            Vector2f scale = {1, 1};
            using detail::Sprite::parent;
            using detail::Sprite::area;
            using detail::Sprite::cache;
        };

        struct Atlas : public detail::Atlas
        {
            Sprite get_sprite(std::string path)
            {
                return {this, path_map[path]};
            }

        private:
            using detail::Atlas::handle;
            using detail::Atlas::path_map;
        };
    }
}