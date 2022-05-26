#pragma once

namespace cl
{
    template <typename T>
    class pointer_proxy
    {
    public:
        template <typename... Args>
        pointer_proxy(Args... _args)
        {
            pointer = new T(_args...);
        }

        ~pointer_proxy()
        {
            delete pointer;
            pointer = nullptr;
        }

        T* pointer = nullptr;
    };
}