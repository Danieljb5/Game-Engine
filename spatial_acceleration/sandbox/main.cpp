#include <spatial_acceleration.hpp>

int main()
{
    LibraryManager libManager;
    cl::sa::detail::load_lib(&libManager);
    cl::log::detail::load_lib(&libManager);

    void* container = cl::sa::create_storage({0, 0}, {100000, 100000});
    
    std::vector<cl::GameObject*> gos;

    for(int i = 0; i < 1000; i++)
    {
        gos.push_back(new cl::GameObject());
        gos.back()->Transform().position.x = i * 10;
        cl::sa::insert(container, *gos.back());
    }
    gos.clear();

    auto search = cl::sa::search(container, {0, 0}, {5000, 50});
    cl::log::info(std::string("Got: ") + std::to_string(search.size()) + std::string(" out of 1000 objects when searching half area"));

    cl::log::info("Removing half of objects...");

    for(auto obj = cl::sa::begin(container); obj != cl::sa::end(container); obj++)
    {
        if(obj == cl::sa::end(container)) break;
        cl::sa::erase(container, obj++); // new iterator is selected, then old iterator is destroyed (if obj is erased and then incremented, a runtime error will happen)
        if(obj == cl::sa::end(container)) break;
    }

    search = cl::sa::search(container, {0, 0}, {5000, 50});
    cl::log::info(std::string("Got: ") + std::to_string(search.size()) + std::string(" out of 500 objects when searching half area"));

    cl::log::info("Clearing tree...");

    cl::sa::clear(container);

    search = cl::sa::search(container, {0, 0}, {5000, 50});
    cl::log::info(std::string("Got: ") + std::to_string(search.size()) + std::string(" out of 0 objects when searching half area"));

    return 0;
}