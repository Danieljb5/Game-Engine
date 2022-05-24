#include <library_loader.hpp>
#include <string>
#include <thread>
#include <unordered_map>

std::unordered_map<std::thread::id, std::string> id_map;
std::unordered_map<std::string, std::thread> thread_map;

extern "C" void init(LibraryManager* libManager)
{
    id_map[std::this_thread::get_id()] = "main";
}

extern "C" std::string get_name()
{
    auto id = std::this_thread::get_id();
    return id_map[id];
}

extern "C" void join(std::string name)
{
    if(thread_map[name].joinable())
    {
        auto id = thread_map[name].get_id();
        thread_map[name].join();
        id_map.erase(id);
        thread_map.erase(name);
    }
}

extern "C" void new_thread(std::string name, void (*__f)())
{
    thread_map[name] = std::thread(__f);
    id_map[thread_map[name].get_id()] = name;
}