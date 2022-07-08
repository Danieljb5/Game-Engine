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

    cl::GameObject go2;
    go2.AddComponent(new cl::DenseTileMap<16, 16>());
    auto& dtm = go2.GetComponent<cl::DenseTileMap<16, 16>>();
    dtm(5, 5) = 64;
    cl::log::info("Dense tilemap at (5, 5): "s + std::to_string(dtm(5, 5)));
    go2.AddComponent(new cl::SparseTileMap<16>());
    auto& stm = go2.GetComponent<cl::SparseTileMap<16>>();

    ((cl::detail::GameObject*)&go2)->_start();
    ((cl::detail::GameObject*)&go2)->_update();

    cl::log::info("Sparse tilemap inserting 54 at (54235, 13253)"s);
    stm(54235, 13253) = 54;

    ((cl::detail::GameObject*)&go2)->_update();

    cl::log::info("Sparse tilemap at (0, 0): "s + std::to_string(stm(0, 0)));

    ((cl::detail::GameObject*)&go2)->_update();

    cl::log::info("Sparse tilemap at (32154, 65347): "s + std::to_string(stm(32154, 65347)));

    ((cl::detail::GameObject*)&go2)->_update();

    cl::log::info("Sparse tilemap at (54235, 13253): "s + std::to_string(stm(54235, 13253)));

    for(int i = 0; i < 100; i++)
    {
        ((cl::detail::GameObject*)&go2)->_update();
    }

    return 0;
}