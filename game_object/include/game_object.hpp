#pragma once

#include <log/include/log.hpp>
#include <vector2/include/vector2.hpp>
#include <assets/include/assets.hpp>
#include <vector>
#include <list>
#include <unordered_map>

namespace cl
{
    // struct Tile
    // {
    //     uint32_t value;
    // };

    // struct TileTemp : public Tile
    // {
    //     Vector2i position;
    // };

    class GameObject;
    struct Transform;

    namespace detail
    {
        cl::Transform& _go_transform(cl::GameObject* go);
        template <typename T>
        bool _go_has_component(cl::GameObject* go);
        template <typename T>
        T& _go_get_component(cl::GameObject* go);
        template <typename T>
        std::vector<T>* _go_get_components(cl::GameObject* go);

        struct ComponentBase
        {
            ComponentBase()
            {

            }

            ~ComponentBase()
            {

            }

            void _set_parent(cl::GameObject* parent)
            {
                this->parent = parent;
            }

        protected:
            cl::GameObject* parent;
        };

    }

    struct ComponentBase : public detail::ComponentBase
    {
        ComponentBase()
        {

        }

        ~ComponentBase()
        {

        }

        virtual void Start() {}
        virtual void Update() {}
        virtual void LateUpdate() {}
        virtual void OnDraw() {}
        virtual void OnCreate() {}
        virtual void OnDestroy() {}

        cl::GameObject& GameObject()
        {
            return *parent;
        }

    protected:
        cl::Transform& Transform() const
        {
            return detail::_go_transform(parent);
        }

        template <typename T>
        bool HasComponent()
        {
            return detail::_go_has_component<T>(parent);
        }

        template <typename T>
        T& GetComponent()
        {
            return detail::_go_get_component<T>(parent);
        }

        template <typename T>
        std::vector<T&> GetComponents()
        {
            return detail::_go_get_components<T>(parent);
        }

    private:
        using detail::ComponentBase::_set_parent;
    };

    namespace detail
    {
        class GameObject
        {
        public:
            GameObject()
            {
                
            }

            ~GameObject()
            {
                
            }

            virtual void _start() = 0;
            virtual void _update() = 0;
            virtual void _late_update() = 0;
            virtual void _on_draw() = 0;

            GameObject(const GameObject& go) = delete;
            GameObject& operator=(const GameObject& go) = delete;

        protected:
            std::vector<cl::ComponentBase*> components;
        };
    }

    struct Transform : public ComponentBase
    {
        Transform()
        {

        }

        ~Transform()
        {

        }

        Vector2<float> position = 0_v;
        Vector2<float> scale = 1_v;
    };

    struct TileMapBase : public ComponentBase
    {
        struct TileSet
        {
            assets::Sprite& operator[](uint16_t tile)
            {
                if(!tileset.count(tile))
                {
                    cl::log::fatal("Request sprite for uninitialised tile "s + std::to_string(tile));
                }
                return tileset[tile];
            }

            void add_sprite(uint16_t tile, assets::Sprite& spr)
            {
                tileset[tile] = spr;
            }

        private:
            std::unordered_map<uint16_t, assets::Sprite> tileset;
        };

        bool solid_tiles[((uint16_t) - 1) + 1] = {false};
        TileSet set;
        Vector2f tile_size = 1_v;

        virtual uint16_t& operator()(size_t x, size_t y)
        {
            cl::log::fatal("tried to get map values from a base map");
            exit(1);
        };
    };

    template <size_t Width, size_t Height>
    struct DenseTileMap : public TileMapBase
    {
        DenseTileMap()
        {
            
        }

        ~DenseTileMap()
        {

        }

        uint16_t& operator()(size_t x, size_t y) override
        {
            if(x < 0 || x >= Width || y < 0 || y >= Height)
            {
                cl::log::fatal("Out of bounds tilemap access at ("s + (std::string)cl::Vector2i(x, y) + ")"s);
            }
            return tiles[x][y];
        }

    private:
        uint16_t tiles[Width][Height] = {0};
    };

    template <size_t ChunkSize>
    struct SparseTileMap : public TileMapBase
    {
        SparseTileMap()
        {
            chunks.insert({0, new chunk()});
        }

        ~SparseTileMap()
        {

        }

        uint16_t& operator()(size_t x, size_t y) override
        {
            const uint32_t chunk_x = (uint32_t)(x / ChunkSize);
            const uint32_t chunk_y = (uint32_t)(y / ChunkSize);
            const size_t key = (((size_t)chunk_x) << 32) + chunk_y;
            if(!chunks.count(key)) chunks.insert({key, new chunk()});
            const size_t off_x = x - (chunk_x * ChunkSize);
            const size_t off_y = y - (chunk_y * ChunkSize);
            return (*chunks[key])(off_x, off_y);
        }

        void Update() override
        {
            cleanup_iterator = chunks.find(cleanup_key);
            if(cleanup_iterator == chunks.end())
            {
                cl::log::fatal("Sparse tilemap iterator is invalid");
                cl::log::fatal("Current tilemap key is "s + std::to_string(cleanup_key));
                cl::log::fatal("Allowing program to create segfault for debugging");
            }
            chunk* c = cleanup_iterator->second;
            bool has_value = false;
            for(size_t x = 0; x < ChunkSize; x++)
            {
                for(size_t y = 0; y < ChunkSize; y++)
                {
                    if((*c)(x, y) != 0)
                    {
                        has_value = true;
                        break;
                    }
                }
            }
            if(!has_value)
            {
                if(chunks.size() > 1)
                {
                    auto it = cleanup_iterator;
                    cleanup_iterator++;
                    if(cleanup_iterator == chunks.end()) cleanup_iterator = chunks.begin();
                    cleanup_key = cleanup_iterator->first;
                    chunks.erase(it);
                }
                else
                {
                    cleanup_iterator++;
                    if(cleanup_iterator == chunks.end()) cleanup_iterator = chunks.begin();
                    cleanup_key = cleanup_iterator->first;
                }
            }
            else
            {
                cleanup_iterator++;
                if(cleanup_iterator == chunks.end()) cleanup_iterator = chunks.begin();
                cleanup_key = cleanup_iterator->first;
            }
        }

        struct chunk
        {
            uint16_t& operator()(size_t x, size_t y)
            {
                return tiles[x][y];
            }
            uint16_t tiles[ChunkSize][ChunkSize] = {0};
        };

    private:
        typename std::unordered_map<size_t, chunk*> chunks;
        typename std::unordered_map<size_t, chunk*>::iterator cleanup_iterator;
        size_t cleanup_key = 0;
    };

    class GameObject : public detail::GameObject
    {
    public:
        GameObject() : detail::GameObject()
        {
            AddComponent(new cl::Transform());
        }

        ~GameObject()
        {
            for(int i = 0; i < components.size(); i++)
            {
                components[i]->OnDestroy();
                delete components[i];
            }
            components.clear();
        }

        void AddComponent(ComponentBase* component)
        {
            components.push_back(component);
            ((detail::ComponentBase*)component)->_set_parent(this);
            component->OnCreate();
        }

        cl::Transform& Transform() const
        {
            return *(cl::Transform*)components[0];
        }
        
        template <typename T>
        bool HasComponent()
        {
            for(int i = 0; i < components.size(); i++)
            {
                if(dynamic_cast<T*>(components[i]) != nullptr) return true;
            }
            return false;
        }

        template <typename T>
        T& GetComponent()
        {
            for(int i = 0; i < components.size(); i++)
            {
                if(dynamic_cast<T*>(components[i]) != nullptr)
                {
                    return *(T*)components[i];
                }
            }
            log::fatal("Could not find component '" + std::string(typeid(T).name()) + std::string("'"));
            exit(1);
        }

        template <typename T>
        std::vector<T&> GetComponents()
        {
            std::vector<T&> result;
            for(int i = 0; i < components.size(); i++)
            {
                if(dynamic_cast<T*>(components[i]) != nullptr)
                {
                    result.push_back(*(T*)components[i]);
                }
            }
            return result;
        }

        GameObject(const GameObject& go) = delete;
        GameObject& operator=(const GameObject& go) = delete;

    private:
        std::vector<ComponentBase*> components;

        void _start() override
        {
            for(auto c : components)
            {
                c->Start();
            }
        }

        void _update() override
        {
            for(auto c : components)
            {
                c->Update();
            }
        }

        void _late_update() override
        {
            for(auto c : components)
            {
                c->LateUpdate();
            }
        }

        void _on_draw() override
        {
            for(auto c : components)
            {
                c->OnDraw();
            }
        }
    };

    Transform& detail::_go_transform(cl::GameObject* go)
    {
        return go->Transform();
    }

    template <typename T>
    bool detail::_go_has_component(cl::GameObject* go)
    {
        return go->HasComponent<T>();
    }

    template <typename T>
    T& detail::_go_get_component(cl::GameObject* go)
    {
        return go->GetComponent<T>();
    }

    template <typename T>
    std::vector<T>* detail::_go_get_components(cl::GameObject* go)
    {
        return go->GetComponents<T>();
    }
}
