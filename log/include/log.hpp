#pragma once

#include <base/library_loader.hpp>
#include <string>

using namespace std::string_literals;

namespace cl
{
    namespace log
    {
        LIBRARY(cl_log)

        STUB_A(set_log_time, void, bool value)
        return set_log_time(value);
        STUB_B(set_log_time, void, bool value)

        STUB_A(set_log_thread, void, bool value)
        return set_log_thread(value);
        STUB_B(set_log_thread, void, bool value)

        STUB_A(fatal, void, const std::string& message)
        return fatal(message);
        STUB_B(fatal, void, const std::string& message)

        STUB_A(error, void, const std::string& message)
        return error(message);
        STUB_B(error, void, const std::string& message)

        STUB_A(warn, void, const std::string& message)
        return warn(message);
        STUB_B(warn, void, const std::string& message)

        STUB_A(info, void, const std::string& message)
        return info(message);
        STUB_B(info, void, const std::string& message)

        STUB_A(debug, void, const std::string& message)
        return debug(message);
        STUB_B(debug, void, const std::string& message)

        STUB_A(trace, void, const std::string& message)
        return trace(message);
        STUB_B(trace, void, const std::string& message)
    }
}