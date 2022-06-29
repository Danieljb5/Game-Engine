#pragma once

#include <base/library_loader.hpp>

namespace cl
{
    namespace cd
    {
        LIBRARY(cl_cd)

        STUB(create_sigsegv, void)
    }
}