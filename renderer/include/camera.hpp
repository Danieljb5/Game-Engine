#pragma once

#include <game_object/include/game_object.hpp>


namespace cl
{
    namespace renderer
    {
        struct Camera;

        extern void (*set_global_camera)(cl::renderer::Camera* cam);

        struct Camera : public ComponentBase
        {
            void SetAsMainCamera()
            {
                set_global_camera(this);
            }

            float zoom = 1;
        };
    }
}