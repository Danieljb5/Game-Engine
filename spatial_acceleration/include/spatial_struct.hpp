#pragma once

namespace cl
{
    namespace sa
    {
        struct Rect
        {
            cl::Vector2d position;
            cl::Vector2d size;

            Rect(const cl::Vector2d& position = {0.0f, 0.0f}, const cl::Vector2d& size = {1.0f, 1.0f})
            : position(position), size(size) {}

            constexpr bool contains(const cl::Vector2d& position) const
            {
                return !(position.x < this->position.x || position.y < this->position.y || position.x >= (this->position.x + size.x) || position.y >= (this->position.y + size.y));
            }

            constexpr bool contains(const Rect& r) const
            {
                return (r.position.x >= position.x) && (r.position.x + r.size.x < position.x + size.x) && (r.position.y >= position.y) && (r.position.y + r.size.y < position.y + size.y);
            }

            constexpr bool overlaps(const Rect& r) const
            {
                return (position.x < r.position.x + r.size.x && position.x + size.x >= r.position.x && position.y < r.position.y + r.size.y && position.y + size.y >= r.position.y);
            }
        };

        template <typename T>
        struct QuadTreeItemLocation
        {
            typename std::list<std::pair<T, Rect>>* container;
            typename std::list<std::pair<T, Rect>>::iterator iterator;
        };

        template <typename T>
        struct QuadTreeItem
        {
            T* item;
            QuadTreeItemLocation<typename std::list<QuadTreeItem<T>>::iterator> location;

            T* operator->() const
            {
                return item;
            }
        };

        struct ItemIterator
        {
            ItemIterator(std::list<QuadTreeItem<cl::GameObject>>::iterator iterator)
            {
                this->iterator = iterator;
            }

            ItemIterator()
            {
                
            }

            std::list<QuadTreeItem<cl::GameObject>>::iterator iterator;

            GameObject* operator->() const
            {
                return iterator->item;
            }

            operator std::list<QuadTreeItem<cl::GameObject>>::iterator&()
            {
                return this->iterator;
            }

            bool operator==(const ItemIterator& it)
            {
                return iterator == it.iterator;
            }

            bool operator!=(const ItemIterator& it)
            {
                return iterator != it.iterator;
            }

            ItemIterator& operator++()
            {
                iterator++;
                return *this;
            }

            ItemIterator operator++(int)
            {
                ItemIterator copy = *this;
                iterator++;
                return copy;
            }

            ItemIterator& operator--()
            {
                iterator--;
                return *this;
            }

            ItemIterator operator--(int)
            {
                ItemIterator copy = *this;
                iterator--;
                return copy;
            }
        };

        struct ConstItemIterator
        {
            ConstItemIterator(std::list<QuadTreeItem<cl::GameObject>>::const_iterator iterator)
            {
                this->iterator = iterator;
            }

            ConstItemIterator()
            {
                
            }

            std::list<QuadTreeItem<cl::GameObject>>::const_iterator iterator;

            const GameObject* operator->() const
            {
                return iterator->item;
            }

            operator std::list<QuadTreeItem<cl::GameObject>>::const_iterator&()
            {
                return this->iterator;
            }

            bool operator==(const ItemIterator& it)
            {
                return iterator == it.iterator;
            }

            bool operator==(const ConstItemIterator& it)
            {
                return iterator == it.iterator;
            }

            bool operator!=(const ItemIterator& it)
            {
                return iterator != it.iterator;
            }

            bool operator!=(const ConstItemIterator& it)
            {
                return iterator != it.iterator;
            }

            ConstItemIterator& operator++()
            {
                iterator++;
                return *this;
            }

            ConstItemIterator operator++(int)
            {
                ConstItemIterator copy = *this;
                iterator++;
                return copy;
            }

            ConstItemIterator& operator--()
            {
                iterator--;
                return *this;
            }

            ConstItemIterator operator--(int)
            {
                ConstItemIterator copy = *this;
                iterator--;
                return copy;
            }
        };
    }
}