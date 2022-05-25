#pragma once

#include <library_loader.hpp>
#include <ts_queue.hpp>
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

        STUB_RET(start_time_ns, const uint64_t)
        STUB_RET(absolute_time_ns, const uint64_t)
        STUB_RET(current_time_ns, const uint64_t)
        STUB_RET(delta_time_ns, const uint64_t)
        STUB(calculate_delta_time, void)
        STUB_RET(start_time, const double)
        STUB_RET(absolute_time, const double)
        STUB_RET(current_time, const double)
        STUB_RET(delta_time, const double)

        STUB_A(sleep_ns, void, const uint64_t& nanoseconds)
        return sleep_ns(nanoseconds);
        STUB_B(sleep_ns, void, const uint64_t& nanoseconds)

        STUB_A(wait_ns, void, const uint64_t& nanoseconds)
        return wait_ns(nanoseconds);
        STUB_B(wait_ns, void, const uint64_t& nanoseconds)

        STUB_A(lazy_wait_ns, void, const uint64_t& nanoseconds, const uint64_t& sleep_interval)
        return lazy_wait_ns(nanoseconds, sleep_interval);
        STUB_B(lazy_wait_ns, void, const uint64_t& nanoseconds, const uint64_t& sleep_interval)

        STUB_A(sleep_until_ns, void, const bool& condition, const uint64_t& sleep_interval)
        return sleep_until_ns(condition, sleep_interval);
        STUB_B(sleep_until_ns, void, const bool& condition, const uint64_t& sleep_interval)

        STUB_A(wait_until_ns, void, const bool& condition, const uint64_t& wait_interval)
        return wait_until_ns(condition, wait_interval);
        STUB_B(wait_until_ns, void, const bool& condition, const uint64_t& wait_interval)

        STUB_A(sleep, void, const double& seconds)
        return sleep(seconds);
        STUB_B(sleep, void, const double& seconds)

        STUB_A(wait, void, const double& seconds)
        return wait(seconds);
        STUB_B(wait, void, const double& seconds)

        STUB_A(lazy_wait, void, const double& seconds, const double& sleep_interval)
        return lazy_wait(seconds, sleep_interval);
        STUB_B(lazy_wait, void, const double& seconds, const double& sleep_interval)

        STUB_A(sleep_until, void, const bool& condition, const double& sleep_interval)
        return sleep_until(condition, sleep_interval);
        STUB_B(sleep_until, void, const bool& condition, const double& sleep_interval)

        STUB_A(wait_until, void, const bool& condition, const double& wait_interval)
        return wait_until(condition, wait_interval);
        STUB_B(wait_until, void, const bool& condition, const double& wait_interval)

        class scoped_sleep
        {
        public:
            scoped_sleep(const double& seconds)
            {
                start_time = current_time();
                end_time = start_time + seconds;
            }

            ~scoped_sleep()
            {
                const double c_time = current_time();
                sleep(end_time - c_time);
            }

        private:
            double start_time;
            double end_time;
        };

        class scoped_wait
        {
        public:
            scoped_wait(const double& seconds)
            {
                start_time = current_time();
                end_time = start_time + seconds;
            }

            ~scoped_wait()
            {
                const double c_time = current_time();
                wait(end_time - c_time);
            }

        private:
            double start_time;
            double end_time;
        };

        class scoped_lazy_wait
        {
        public:
            scoped_lazy_wait(const double& seconds, const double& sleep_interval)
            {
                start_time = current_time();
                end_time = start_time + seconds;
                this->sleep_interval = sleep_interval;
            }

            ~scoped_lazy_wait()
            {
                const double c_time = current_time();
                lazy_wait(end_time - c_time, sleep_interval);
            }

        private:
            double start_time;
            double end_time;
            double sleep_interval;
        };
    }
}