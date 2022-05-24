#include <log.hpp>

int main()
{
    LibraryManager libManager;
    cl::log::detail::load_lib(&libManager);

    cl::log::trace("Hello there");
    cl::log::debug("Hello there");
    cl::log::info("Hello there");
    cl::log::warn("Hello there");
    cl::log::error("Hello there");
    cl::log::fatal("Hello there");

    cl::log::set_log_time(true);

    cl::log::trace("Hello there");
    cl::log::debug("Hello there");
    cl::log::info("Hello there");
    cl::log::warn("Hello there");
    cl::log::error("Hello there");
    cl::log::fatal("Hello there");

    cl::log::set_log_time(false);
    cl::log::set_log_thread(true);

    cl::log::trace("Hello there");
    cl::log::debug("Hello there");
    cl::log::info("Hello there");
    cl::log::warn("Hello there");
    cl::log::error("Hello there");
    cl::log::fatal("Hello there");

    cl::log::set_log_time(true);

    cl::log::trace("Hello there");
    cl::log::debug("Hello there");
    cl::log::info("Hello there");
    cl::log::warn("Hello there");
    cl::log::error("Hello there");
    cl::log::fatal("Hello there");

    return 0;
}