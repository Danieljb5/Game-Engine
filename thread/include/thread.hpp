#pragma once

#include <library_loader.hpp>
#include <string>
#include <iostream>

namespace cl
{
    namespace thread
    {
        LIBRARY(cl_thread)

        STUB_RET(get_name, std::string)

        STUB_A(join, void, std::string name)
        return join(name);
        STUB_B(join, void, std::string name)

        STUB_A(new_thread, void, std::string name, void (*__f)())
        return new_thread(name, __f);
        STUB_B(new_thread, void, std::string name, void (*__f)())
    }
}