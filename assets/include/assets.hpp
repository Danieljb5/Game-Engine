#pragma once

#include <base/library_loader.hpp>
#include "atlas.hpp"
#include <vector2/include/vector2.hpp>
#include <vector>

namespace cl
{
    namespace assets
    {
        LIBRARY(cl_assets)

        STUB_A(generate_atlas, std::vector<cl::assets::Atlas>, const std::string& path, const Vector2u& size)
        return generate_atlas(path, size);
        STUB_B_RET(generate_atlas, std::vector<cl::assets::Atlas>, const std::string& path, const Vector2u& size)

        STUB_A(set_renderer, void, SDL_Renderer* renderer)
        return set_renderer(renderer);
        STUB_B(set_renderer, void, SDL_Renderer* renderer)

        STUB_A(set_window_surface, void, SDL_Surface* surf)
        return set_window_surface(surf);
        STUB_B(set_window_surface, void, SDL_Surface* surf)
    }
}