#include <platform.hpp>
#ifdef WINDOWS

#include <library_loader.hpp>
#include <thread.hpp>
#include <windows.h>
#include <chrono>
#include <ctime>
#include <mutex>

#ifndef LOG_DEBUG_ENABLE
#define LOG_DEBUG_ENABLE 1
#endif
#ifndef LOG_TRACE_ENABLE
#define LOG_TRACE_ENABLE 1
#endif


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

const uint8_t channel_col[6] =
{
    64, 4, 6, 2, 1, 8
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
    std::string log_message = log_fmt + std::string(message) + std::string("\n");
    mux.lock();
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h_console, channel_col[level]);
    LPDWORD num_written = 0;
    WriteConsoleA(h_console, log_message.c_str(), (DWORD)log_message.size(), num_written, 0);
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
    void cl::log::trace(const std::string& message) {}
    #endif
}

#endif