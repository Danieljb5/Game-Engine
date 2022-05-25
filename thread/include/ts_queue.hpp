#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>

namespace cl
{
    namespace thread
    {
        template <typename T>
        class ThreadSafeQueue
        {
        public:
            ThreadSafeQueue() = default;
            ThreadSafeQueue(const ThreadSafeQueue<T>&) = delete;

            virtual ~ThreadSafeQueue()
            {
                clear();
            }

            const T& front()
            {
                std::scoped_lock lock(m_mux_queue);
                return m_queue.front();
            }

            const T& back()
            {
                std::scoped_lock lock(m_mux_queue);
                return m_queue.back();
            }

            void push_back(const T& item)
            {
                std::scoped_lock lock(m_mux_queue);
                m_queue.emplace_back(std::move(item));
                std::unique_lock<std::mutex> ul(m_mux_cv);
                cv.notify_one();
            }

            void push_front(const T& item)
            {
                std::scoped_lock lock(m_mux_queue);
                m_queue.emplace_front(std::move(item));
                std::unique_lock<std::mutex> ul(m_mux_cv);
                cv.notify_one();
            }

            bool empty()
            {
                std::scoped_lock lock(m_mux_queue);
                return m_queue.empty();
            }

            size_t count()
            {
                std::scoped_lock lock(m_mux_queue);
                return m_queue.size();
            }

            void clear()
            {
                std::scoped_lock lock(m_mux_queue);
                m_queue.clear();
            }

            T pop_front()
            {
                std::scoped_lock lock(m_mux_queue);
                auto t = std::move(m_queue.front());
                m_queue.pop_front();
                return t;
            }

            T pop_back()
            {
                std::scoped_lock lock(m_mux_queue);
                auto t = std::move(m_queue.back());
                m_queue.pop_back();
                return t;
            }

            void wait()
            {
                while(empty())
                {
                    std::unique_lock<std::mutex> ul(m_mux_cv);
                    cv.wait(ul);
                }
            }

        protected:
            std::mutex m_mux_queue;
            std::deque<T> m_queue;
            std::condition_variable cv;
            std::mutex m_mux_cv;
        };
    }
}