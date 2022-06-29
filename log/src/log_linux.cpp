#include <base/platform.hpp>
#ifdef LINUX

#include <base/library_loader.hpp>
#include <thread/include/thread.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
#include <mutex>

std::string get_time_fmt()
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string s(30, '\0');
    uint64_t size = std::strftime(&s[0], s.size(), "%d/%m/%Y %H:%M:%S", std::localtime(&now));
    s.resize(size);
    return s;
}

const std::string channel_str[6] =
{
    "[FATAL]: ",
    "[ERROR]: ",
    "[WARN]:  ",
    "[INFO]:  ",
    "[DEBUG]: ",
    "[TRACE]: "
};

const char* channel_col[6] =
{
    "\033[38;5;0;48;5;9m",
    "\033[38;5;9;48;5;0m",
    "\033[38;5;11;48;5;0m",
    "\033[38;5;10;48;5;0m",
    "\033[38;5;12;48;5;0m",
    "\033[38;5;8;48;5;0m"
};

std::mutex mux;
bool log_time = false, log_thread = false;


enum log_level
{
    L_FATAL, L_ERROR, L_WARN, L_INFO, L_DEBUG, L_TRACE
};

void log(const char* message, const log_level level)
{
    std::string log_fmt = "";
    if(log_thread) log_fmt = cl::thread::get_name() + " | ";
    if(log_time) log_fmt += get_time_fmt() + " ";
    log_fmt += channel_str[level];
    mux.lock();
    std::cout << channel_col[level] << log_fmt << message << "\033[0m\n";
    mux.unlock();
}

extern "C"
{
    void init(LibraryManager* libManager)
    {
        cl::thread::detail::load_lib(libManager);
    }

    void set_log_time(bool value)
    {
        log_time = value;
    }

    void set_log_thread(bool value)
    {
        log_thread = value;
    }

    void fatal(const std::string& message)
    {
        log(message.c_str(), L_FATAL);
    }

    void error(const std::string& message)
    {
        log(message.c_str(), L_ERROR);
    }

    void warn(const std::string& message)
    {
        log(message.c_str(), L_WARN);
    }

    void info(const std::string& message)
    {
        log(message.c_str(), L_INFO);
    }

    #ifndef DIST
    void debug(const std::string& message)
    {
        log(message.c_str(), L_DEBUG);
    }
    #else
    void debug(const std::string& message) {}
    #endif

    #ifndef DIST
    void trace(const std::string& message)
    {
        log(message.c_str(), L_TRACE);
    }
    #else
    void trace(const std::string& message) {}
    #endif
}

#endif