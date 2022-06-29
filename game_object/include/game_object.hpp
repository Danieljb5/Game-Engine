#pragma once

#include <log.hpp>
#include <vector2.hpp>
#include <vector>

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

    namespace detail
    {
        template <typename T>
        bool _go_has_component(GameObject* go);
        template <typename T>
        T* _go_get_component(GameObject* go);
        template <typename T>
        std::vector<T>* _go_get_components(GameObject* go);

        struct ComponentBase
        {
            ComponentBase()
            {

            }

            ~ComponentBase()
            {

            }

            void _set_parent(GameObject* parent)
            {
                this->parent = parent;
            }

        protected:
            GameObject* parent;
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
        virtual void OnCreate() {}
        virtual void OnDestroy() {}

        cl::GameObject& GameObject()
        {
            return *parent;
        }

    protected:
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

    struct Transform : public ComponentBase
    {
        Transform()
        {

        }

        ~Transform()
        {

        }

        Vector2<float> position;
        Vector2<float> scale;
        float rotation;
    };

    class GameObject
    {
    public:
        GameObject()
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

        cl::Transform& Transform() const
        {
            return *(cl::Transform*)components[0];
        }

        void AddComponent(ComponentBase* component)
        {
            components.push_back(component);
            ((detail::ComponentBase*)component)->_set_parent(this);
            component->OnCreate();
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
    };

    template <typename T>
    bool detail::_go_has_component(GameObject* go)
    {
        return go->HasComponent<T>();
    }

    template <typename T>
    T* detail::_go_get_component(GameObject* go)
    {
        return go->GetComponent<T>();
    }

    template <typename T>
    std::vector<T>* detail::_go_get_components(GameObject* go)
    {
        return go->GetComponents<T>();
    }
}
