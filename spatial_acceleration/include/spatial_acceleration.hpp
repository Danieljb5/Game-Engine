#pragma once

#include <library_loader.hpp>
#include <vector2.hpp>
#include <list>
#include <game_object.hpp>
#include <spatial_struct.hpp>

namespace cl
{
    namespace sa
    {
        LIBRARY(cl_sa)

        STUB_A(create_storage, void*, const cl::Vector2d& position, const cl::Vector2d& size)
        return create_storage(position, size);
        STUB_B_RET_PTR(create_storage, void*, const cl::Vector2d& position, const cl::Vector2d& size)

        STUB_A(resize, void, void* container, const cl::Vector2d& position, const cl::Vector2d& size)
        return resize(container, position, size);
        STUB_B(resize, void, void* container, const cl::Vector2d& position, const cl::Vector2d& size)

        STUB_A(size, size_t, void* container)
        return size(container);
        STUB_B_RET(size, size_t, void* container)

        STUB_A(empty, bool, void* container)
        return empty(container);
        STUB_B_RET(empty, bool, void* container);

        STUB_A(clear, void, void* container)
        clear(container);
        STUB_B(clear, void, void* container)

        STUB_A(begin, ItemIterator, void* container)
        return begin(container);
        STUB_B_RET(begin, ItemIterator, void* container)

        STUB_A(end, ItemIterator, void* container)
        return end(container);
        STUB_B_RET(end, ItemIterator, void* container)

        STUB_A(cbegin, ConstItemIterator, void* container)
        return cbegin(container);
        STUB_B_RET(cbegin, ConstItemIterator, void* container)

        STUB_A(cend, ConstItemIterator, void* container)
        return cend(container);
        STUB_B_RET(cend, ConstItemIterator, void* container)

        STUB_A(insert, void, void* container, const cl::GameObject& item)
        return insert(container, item);
        STUB_B(insert, void, void* container, const cl::GameObject& item)

        STUB_A(search, std::list<ItemIterator>, void* container, const cl::Vector2d& position, const cl::Vector2d& size)
        return search(container, position, size);
        STUB_B_RET(search, std::list<ItemIterator>, void* container, const cl::Vector2d& position, const cl::Vector2d& size)

        STUB_A(erase, void, void* container, ItemIterator item)
        return erase(container, item);
        STUB_B(erase, void, void* container, ItemIterator item)

        STUB_A(relocate, void, void* container, ItemIterator item)
        return relocate(container, item);
        STUB_B(relocate, void, void* container, ItemIterator item)
    }
}