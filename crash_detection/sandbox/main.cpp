#include <base/entry_setup.hpp>
#include <crash_detection/include/crash_detection.hpp>

int main()
{
    cl::cd::create_sigsegv();

    return 0;
}