#include <base/library_loader.hpp>
#include "../include/ts_queue.hpp"
#include <string>
#include <thread>
#include <unordered_map>
#include <chrono>

std::unordered_map<std::thread::id, std::string> id_map;
std::unordered_map<std::string, std::thread> thread_map;
uint64_t start_timestamp_ns, absolute_timestamp_ns, current_timestamp_ns, delta_timestamp_ns, last_delta_timecheck_ns;

extern "C"
{
    void init(LibraryManager* libManager)
    {
        id_map[std::this_thread::get_id()] = "main";
        start_timestamp_ns = std::chrono::system_clock::now().time_since_epoch().count();
        last_delta_timecheck_ns = start_timestamp_ns;
    }

    std::string get_name()
    {
        auto id = std::this_thread::get_id();
        return id_map[id];
    }

    void join(std::string name)
    {
        if(thread_map[name].joinable())
        {
            auto id = thread_map[name].get_id();
            thread_map[name].join();
            id_map.erase(id);
            thread_map.erase(name);
        }
    }

    void new_thread(std::string name, void (*__f)())
    {
        thread_map[name] = std::thread(__f);
        id_map[thread_map[name].get_id()] = name;
    }

    const uint64_t start_time_ns()
    {
        return start_timestamp_ns;
    }

    const uint64_t absolute_time_ns()
    {
        absolute_timestamp_ns = std::chrono::system_clock::now().time_since_epoch().count();
        return absolute_timestamp_ns;
    }

    const uint64_t current_time_ns()
    {
        current_timestamp_ns = std::chrono::system_clock::now().time_since_epoch().count() - start_timestamp_ns;
        return current_timestamp_ns;
    }

    const uint64_t delta_time_ns()
    {
        return delta_timestamp_ns;
    }

    void calculate_delta_time()
    {
        uint64_t curr_time = current_time_ns();
        delta_timestamp_ns = curr_time - last_delta_timecheck_ns;
        last_delta_timecheck_ns = curr_time;
    }

    const double start_time()
    {
        return start_time_ns() * 0.000000001;
    }

    const double absolute_time()
    {
        return absolute_time_ns() * 0.000000001;
    }

    const double current_time()
    {
        return current_time_ns() * 0.000000001;
    }

    const double delta_time()
    {
        return delta_time_ns() * 0.000000001;
    }

    void sleep_ns(const uint64_t& nanoseconds)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
    }

    void wait_ns(const uint64_t& nanoseconds)
    {
        const uint64_t start = current_time_ns();
        const uint64_t end = start + nanoseconds;
        uint64_t curr = start;
        while(curr < end)
        {
            curr = current_time_ns();
        }
    }

    void lazy_wait_ns(const uint64_t& nanoseconds, const uint64_t& sleep_interval)
    {
        const uint64_t start = current_time_ns();
        const uint64_t end = start + nanoseconds;
        uint64_t curr = start;
        while(curr < end)
        {
            sleep_ns(sleep_interval);
            curr = current_time_ns();
        }
    }

    void sleep_until_ns(const bool& condition, const uint64_t& sleep_interval)
    {
        while(!condition)
        {
            sleep_ns(sleep_interval);
        }
    }

    void wait_until_ns(const bool& condition, const uint64_t& wait_interval)
    {
        while(!condition)
        {
            wait_ns(wait_interval);
        }
    }

    void sleep(const double& seconds)
    {
        sleep_ns(seconds * 1000000000);
    }

    void wait(const double& seconds)
    {
        wait_ns(seconds * 1000000000);
    }

    void lazy_wait(const double& seconds, const double& sleep_interval)
    {
        lazy_wait_ns(seconds * 1000000000, sleep_interval * 1000000000);
    }

    void sleep_until(const bool& condition, const double& sleep_interval)
    {
        sleep_until_ns(condition, sleep_interval * 1000000000);
    }

    void wait_until(const bool& condition, const double& wait_interval)
    {
        wait_until_ns(condition, wait_interval * 1000000000);
    }
}