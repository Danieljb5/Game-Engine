#pragma once

#include <log/include/log.hpp>
#include <vector2/include/vector2.hpp>
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
    struct Transform;

    namespace detail
    {
        cl::Transform& _go_transform(cl::GameObject* go);
        template <typename T>
        bool _go_has_component(cl::GameObject* go);
        template <typename T>
        T* _go_get_component(cl::GameObject* go);
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
        float rotation;
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
    T* detail::_go_get_component(cl::GameObject* go)
    {
        return go->GetComponent<T>();
    }

    template <typename T>
    std::vector<T>* detail::_go_get_components(cl::GameObject* go)
    {
        return go->GetComponents<T>();
    }
}
