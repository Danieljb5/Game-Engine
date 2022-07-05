#pragma once

#include <game_object/include/game_object.hpp>
#include <assets/include/atlas.hpp>
#include "renderer.hpp"
#include <log/include/log.hpp>

namespace cl
{
    namespace renderer
    {
        struct SpriteRenderer : public ComponentBase
        {
            SpriteRenderer()
            {

            }

            ~SpriteRenderer()
            {

            }

            cl::assets::GoSprite Sprite;

            void OnDraw() override
            {
                const Vector2f pos = Transform().position;
                if(Sprite.get_position() != pos)
                {
                    Sprite.set_position(pos);
                }

                const Vector2f scale = Transform().scale;
                if(Sprite.get_scale() != scale)
                {
                    Sprite.set_scale(scale);
                }
                
                push_render_stack(Sprite);
            }
        };
    }
}