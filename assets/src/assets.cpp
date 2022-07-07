#include <log/include/log.hpp>
#include "../include/atlas.hpp"
#include <vector2/include/vector2.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <experimental/filesystem>

using namespace cl;
using namespace log;
using namespace assets;

SDL_Renderer* global_renderer;
SDL_Surface* global_window_surface;

struct PB_Rect
{
    Vector2u position;
    Vector2u size;

    PB_Rect(const Vector2u& position = {0, 0}, const Vector2u& size = {1, 1})
    {
        this->position = position;
        this->size = size;
    }
};

struct PackingRect : public PB_Rect
{
    using PB_Rect::PB_Rect;
    SDL_Surface* handle = nullptr;
    std::string name = "";
    bool was_packed = false;
};

struct PackingArea
{
    PackingArea(std::vector<PackingArea*>* areas_handle, PB_Rect area)
    {
        this->areas_handle = areas_handle;
        this->area = area;
        this->rects.clear();
    }

    unsigned int largest_h = 0;
    Vector2u position = {0, 0};
    PB_Rect area;
    std::vector<PackingArea*>* areas_handle = nullptr;
    std::vector<PackingRect> rects;

    bool pack(PackingRect& p, bool is_recursive = false)
    {
        if((position.y + p.size.y) > area.size.y) return false;

        if(position.x + p.size.x > area.size.x)
        {
            if(is_recursive) return false;
            if(rects.size() == 0) return false;
            for(int i = 0; i < rects.size(); i++)
            {
                Vector2u pos = {rects[i].position.x, rects[i].position.y + rects[i].size.y};
                Vector2u size = {rects[i].size.x, largest_h - rects[i].size.y};
                if(size.x > 0 && size.y > 0)
                {
                    PackingArea* new_area = new PackingArea(areas_handle, {pos, size});
                    areas_handle->push_back(new_area);
                }
            }
            Vector2u pos = {rects.back().position.x + rects.back().size.x, rects.back().position.y};
            Vector2u size = {(area.size.x - (rects.back().position.x + rects.back().size.x)), largest_h};
            if(size.x > 0 && size.y > 0)
            {
                PackingArea* new_area = new PackingArea(areas_handle, {pos, size});
                areas_handle->push_back(new_area);
            }
            rects.clear();
            position.y += largest_h;
            position.x = 0;
            largest_h = 0;
            return pack(p, true);
        }

        p.position = area.position + position;
        position.x += p.size.x;
        if(p.size.y > largest_h)
        {
            largest_h = p.size.y;
        }
        p.was_packed = true;
        rects.push_back(p);
        return true;
    }
};

bool load_image(SDL_Renderer* renderer, SDL_Surface*& handle, const std::string& path, Vector2u& size)
{
    handle = IMG_Load(path.c_str());
    size.x = handle->w;
    size.y = handle->h;
    if(!handle)
    {
        log::error("failed to load image '" + path + "', reason: " + IMG_GetError());
        return false;
    }
    return true;
}

void pack(std::vector<PackingRect>& rects, const Vector2u& size)
{
    std::sort(rects.begin(), rects.end(), [](const PackingRect& r1, const PackingRect& r2)
    {
        bool larger = r1.size.y > r2.size.y;
        if(r1.size.y == r2.size.y)
        {
            larger = r1.size.x > r2.size.x;
        }
        return larger;
    });

    std::vector<PackingArea*> areas;
    PackingArea* main_area = new PackingArea(&areas, {{0, 0}, size});
    areas.push_back(main_area);

    for(PackingRect& p : rects)
    {
        for(int i = areas.size() - 1; i >= 0; --i)
        {
            if(areas[i]->pack(p)) break;
        }
    }

    for(auto packing_area : areas)
    {
        packing_area->rects.clear();
        delete packing_area;
    }
    areas.clear();
}

extern "C"
{
    void init(LibraryManager* libManager)
    {
        cl::log::detail::load_lib(libManager);
    }

    Atlas generate_atlas(const std::string& path, const Vector2u& size)
    {
        std::list<std::string> files;
        for(const auto& file : std::experimental::filesystem::recursive_directory_iterator(path))
        {
            std::string extension = file.path().extension().string();
            if(extension == ".png" || extension == ".jpg" || extension == ".bmp")
            {
                files.push_back(file.path().string());
            }
        }

        std::list<PackingRect> prs;
        for(auto& f : files)
        {
            PackingRect pr;
            if(!load_image(global_renderer, pr.handle, f, pr.size))
            {
                cl::log::error("failed to load image '" + pr.name + "'");
                exit(1);
            }
            pr.name = f;
            prs.push_back(pr);
        }

        std::vector<PackingRect> failed;
        for(auto& p : prs) failed.push_back(p);

        std::vector<PackingRect> packed;
        pack(failed, size);

        for(int i = 0; i < failed.size(); i++)
        {
            if(failed[i].was_packed)
            {
                packed.push_back(failed[i]);
                failed[i] = failed.back();
                failed.pop_back();
                i--;
            }
        }

        assets::detail::Atlas a;
        SDL_Surface* surf = SDL_CreateRGBSurface(0, size.x, size.y, 32, 0, 0, 0, 0);
        for(auto& p : packed)
        {
            if(p.handle)
            {
                SDL_Rect dst;
                dst.x = p.position.x;
                dst.y = p.position.y;
                dst.w = p.size.x;
                dst.h = p.size.y;
                SDL_BlitSurface(p.handle, nullptr, surf, &dst);
                SDL_FreeSurface(p.handle);
            }
            else
            {
                cl::log::error(p.name + " could not be added to an atlas because it does not exist");
            }
            SDL_Rect area;
            area.x = p.position.x;
            area.y = p.position.y;
            area.w = p.size.x;
            area.h = p.size.y;
            std::string new_name = p.name;
            std::replace(new_name.begin(), new_name.end(), '\\', '/');
            a.path_map.insert({new_name, area});
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(global_renderer, surf);
        if(!tex)
        {
            cl::log::error("failed to create atlas texture");
        }

        SDL_FreeSurface(surf);
        a.handle = tex;
        Atlas out = *(assets::Atlas*)&a;
        if(failed.size() > 0)
        {
            log::error("failed to pack "s + std::to_string(failed.size()) + " sprites, please expand the atlas."s);
        }
        return out;
    }

    void set_renderer(SDL_Renderer* renderer)
    {
        global_renderer = renderer;
    }

    void set_window_surface(SDL_Surface* surf)
    {
        global_window_surface = surf;
    }
}
