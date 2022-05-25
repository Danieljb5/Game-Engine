#include <log.hpp>
#include <thread.hpp>

bool condition = false;

void thread()
{
    cl::log::info("Starting sleep until, eta 1s");
    cl::thread::sleep_until(condition, 0.01);

    condition = false;

    cl::log::info("Starting wait until, eta 1s");
    cl::thread::wait_until(condition, 0.01);

    cl::log::info("Threading tests finished, joining...");
}

int main()
{
    LibraryManager libManager;
    cl::log::detail::load_lib(&libManager);
    cl::thread::detail::load_lib(&libManager);

    cl::log::set_log_thread(true);
    cl::log::set_log_time(true);

    cl::log::info("Starting thread test, testing 1 second sleep");
    double start = cl::thread::current_time();
    cl::thread::sleep(1);
    double duration = cl::thread::current_time() - start;
    cl::log::info("Took " + std::to_string(duration) + " seconds");

    cl::log::info("Starting thread test, testing 1 second wait");
    start = cl::thread::current_time();
    cl::thread::wait(1);
    duration = cl::thread::current_time() - start;
    cl::log::info("Took " + std::to_string(duration) + " seconds");

    cl::log::info("Starting multithreaded test...");
    cl::thread::new_thread("thread", thread);

    cl::thread::sleep(1);
    condition = true;

    while(!condition);
    cl::thread::sleep(1);
    condition = true;

    cl::thread::join("thread");

    cl::log::info("Starting delta time test, approx 1s");
    cl::thread::calculate_delta_time();
    cl::thread::wait(1);
    cl::thread::calculate_delta_time();
    cl::log::info("Recorded delta time: " + std::to_string(cl::thread::delta_time()) + " seconds");

    cl::log::info("All tests completed!");

    return 0;
}