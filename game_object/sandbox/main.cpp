#include <game_object/include/game_object.hpp>
#include <log/include/log.hpp>
#include <vector2/include/vector2.hpp>

class MyScript : public cl::ComponentBase
{
public:
    MyScript()
    {

    }

    ~MyScript()
    {

    }

    void OnCreate()
    {
        cl::log::info("Script OnCreate()");
    }

    void Start()
    {
        cl::log::info("Script Start()");
    }

    void OnDestroy()
    {
        cl::log::info("Script OnDestroy()");
    }
};

int main()
{
    LibraryManager libManager;
    cl::log::detail::load_lib(&libManager);

    cl::GameObject go;
    cl::log::info("GameObject position: " + (std::string)go.Transform().position);
    
    go.Transform().position += cl::Vector2f(10, 4);
    cl::log::info("GameObject position: " + (std::string)go.Transform().position);

    cl::Vector2f a = {0.5, 1.4};
    cl::Vector2f b = {43.1, -3.5};
    cl::Vector2f c = a + b;

    cl::log::info("a: " + (std::string)a);
    cl::log::info("b: " + (std::string)b);
    cl::log::info("c: " + (std::string)c);

    go.AddComponent(new MyScript());

    go.GetComponent<MyScript>().Start();

    return 0;
}