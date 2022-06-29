#pragma once

#include "../log/include/log.hpp"
#include "../crash_detection/include/crash_detection.hpp"
#include "../thread/include/thread.hpp"
#include "../vector2/include/vector2.hpp"
#include "../game_object/include/game_object.hpp"
#include "../spatial_acceleration/include/spatial_acceleration.hpp"

namespace cl
{
    namespace detail
    {
        LibraryManager global_inst;

        class StartupFunctions
        {
        public:
            StartupFunctions()
            {
                log::detail::load_lib(&global_inst);
                cd::detail::load_lib(&global_inst);
                thread::detail::load_lib(&global_inst);
                sa::detail::load_lib(&global_inst);
            }
        };

        StartupFunctions global_startup;
    }
}