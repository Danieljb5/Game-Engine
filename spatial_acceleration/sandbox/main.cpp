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

    int i = 0;
    for(auto obj = cl::sa::begin(container); obj != cl::sa::end(container); obj++)
    {
        if(obj == cl::sa::end(container)) break;
        if(i % 2 == 0) continue;
        cl::sa::erase(container, obj);
        delete obj.iterator->item;
        i++;
    }

    search = cl::sa::search(container, {0, 0}, {5000, 50});
    cl::log::info(std::string("Got: ") + std::to_string(search.size()) + std::string(" out of 500 objects when searching half area"));

    cl::log::info("Clearing tree...");

    for(auto obj = cl::sa::begin(container); obj != cl::sa::end(container); obj++)
    {
        delete obj.iterator->item;
    }

    cl::sa::clear(container);


    search = cl::sa::search(container, {0, 0}, {5000, 50});
    cl::log::info(std::string("Got: ") + std::to_string(search.size()) + std::string(" out of 0 objects when searching half area"));

    return 0;
}