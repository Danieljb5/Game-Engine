#include <game_object/include/game_object.hpp>
#include <vector2/include/vector2.hpp>
#include "../include/spatial_struct.hpp"
#include <list>
#include <algorithm>

// #define GRID_HALF_SIZE 1048576
// #define GRID_LOCAL_HALF_SIZE 16
// #define GRID_LAYERS 4
// #define TOP_LAYER_HALF_SIZE 1
// #define MAX_DEPTH 12
// const cl::Vector2i VEC_GRID_HALF_SIZE = cl::Vector2i(GRID_HALF_SIZE, GRID_HALF_SIZE);

// template <int HalfSize, typename T>
// struct Grid
// {
//     T* children[(HalfSize * 2) * (HalfSize * 2)] = {nullptr};

//     int size()
//     {
//         int count = 0;
//         for(int i = 0; i < sizeof(children) / sizeof(children[0]); i++)
//         {
//             if(children[i]) count++;
//         }
//         return count;
//     }

//     int convert_to_index(cl::Vector2i local_position)
//     {
//         return local_position.x + (local_position.y * (HalfSize * 2));
//     }
// };

// enum StorageType
// {
//     STATIC, DYNAMIC
// };

// class StorageBase
// {
// public:
//     std::vector<void*> m_storage;

//     virtual StorageType get_type() = 0;
//     virtual void insert(void* object) = 0;
//     virtual std::list<void*> search(cl::Vector2d position, cl::Vector2d half_width) = 0;
//     virtual void remove(void* object) = 0;
//     virtual void remove(cl::Vector2d position, cl::Vector2d half_width) = 0;
//     virtual void clear() = 0;
//     virtual void update() = 0; // handled internally by engine
    
//     size_t size()
//     {
//         return m_storage.size();
//     }
// };

// class SpatialHash : public StorageBase
// {
// public:
//     StorageType get_type() override
//     {
//         return STATIC;
//     }

//     void insert(void* object) override
//     {
//         cl::TileTemp* tmp = (cl::TileTemp*)object;
//         cl::Tile* tile = (cl::Tile*)object;
//         cl::Tile*& grid_tile = construct_grid_tile(tmp->position);
//         if(grid_tile != nullptr)
//         {
//             m_storage.erase(std::remove(m_storage.begin(), m_storage.end(), grid_tile), m_storage.end());
//         }
//         grid_tile = tile;
//         m_storage.push_back(grid_tile);
//     }

//     std::list<void*> search(cl::Vector2d position, cl::Vector2d half_width) override
//     {
//         const cl::Vector2i top_right = position + half_width;
//         const cl::Vector2i bottom_left = position - half_width;

//         std::list<void*> result;

//         cl::Vector2i curr_position = bottom_left;
//         while(curr_position != top_right)
//         {
//             curr_position.x++;
//             if(curr_position.x > top_right.x)
//             {
//                 curr_position.x = bottom_left.x;
//                 curr_position.y++;
//             }
//             const cl::Tile* tile = get_grid_tile(curr_position);
//             if(tile != nullptr)
//             {
//                 cl::TileTemp* temp = new cl::TileTemp();
//                 temp->value = tile->value;
//                 temp->position = curr_position;
//                 result.push_back(temp);
//             }
//         }

//         return result;
//     };

//     void remove(void* object) override
//     {
//         cl::TileTemp* tile = (cl::TileTemp*)object;
//         remove(tile->position, {0, 0});
//     }

//     void remove(cl::Vector2d position, cl::Vector2d half_width) override
//     {
//         const cl::Tile* tile = get_grid_tile(position);
//         delete tile;
//         remove_grid_tile(position);
//         remove_queue.push_back({position, 3});
//     }

//     void clear() override
//     {
//         cl::log::fatal("tried to remove an object from a static grid - spatial_acceleration.cpp line " + __LINE__);
//         for(int a = 0; a < sizeof(m_grid.children) / sizeof(m_grid.children[0]); a++)
//         {
//             for(int b = 0; b < sizeof(m_grid.children[a]->children) / sizeof(m_grid.children[a]->children[0]); b++)
//             {
//                 for(int c = 0; c < sizeof(m_grid.children[a]->children[b]->children) / sizeof(m_grid.children[a]->children[b]->children[0]); c++)
//                 {
//                     for(int d = 0; d < sizeof(m_grid.children[a]->children[b]->children[c]->children) / sizeof(m_grid.children[a]->children[b]->children[c]->children[0]); d++)
//                     {
//                         for(int e = 0; e < sizeof(m_grid.children[a]->children[b]->children[c]->children[d]->children) / sizeof(m_grid.children[a]->children[b]->children[c]->children[d]->children[0]); e++)
//                         {
//                             delete m_grid.children[a]->children[b]->children[c]->children[d]->children[e];
//                         }
//                         delete m_grid.children[a]->children[b]->children[c]->children[d];
//                     }
//                     delete m_grid.children[a]->children[b]->children[c];
//                 }
//                 delete m_grid.children[a]->children[b];
//             }
//             delete m_grid.children[a];
//             m_grid.children[a] = nullptr;
//         }
//         m_storage.clear();
//     }

//     void update() override
//     {
//         const size_t queue_size = remove_queue.size();
//         for(int i = 0; i < queue_size; i++)
//         {
//             const auto& elem = remove_queue.front();
//             switch(elem.second)
//             {
//             case 0:
//                 auto grid_index = m_grid.convert_to_index(get_pos_0(elem.first));
//                 auto grid = m_grid.children[grid_index];
//                 if(grid->size() == 0)
//                 {
//                     delete grid;
//                     m_grid.children[grid_index] = nullptr;
//                 }
//                 break;
//             case 1:
//                 auto index1 = m_grid.convert_to_index(get_pos_0(elem.first));
//                 auto parent = m_grid.children[index1];
//                 auto grid_index = parent->convert_to_index(get_pos_1(elem.first));
//                 auto grid = parent->children[grid_index];
//                 if(grid->size() == 0)
//                 {
//                     delete grid;
//                     parent->children[grid_index] = nullptr;
//                 }
//                 remove_queue.push_back({elem.first, elem.second - 1});
//                 break;
//             case 2:
//                 auto index1 = m_grid.convert_to_index(get_pos_0(elem.first));
//                 auto index2 = m_grid.children[index1]->convert_to_index(get_pos_1(elem.first));
//                 auto parent = m_grid.children[index1]->children[index2];
//                 auto grid_index = parent->convert_to_index(get_pos_2(elem.first));
//                 auto grid = parent->children[grid_index];
//                 if(grid->size() == 0)
//                 {
//                     delete grid;
//                     parent->children[grid_index] = nullptr;
//                 }
//                 remove_queue.push_back({elem.first, elem.second - 1});
//                 break;
//             case 3:
//                 auto index1 = m_grid.convert_to_index(get_pos_0(elem.first));
//                 auto index2 = m_grid.children[index1]->convert_to_index(get_pos_1(elem.first));
//                 auto index3 = m_grid.children[index1]->children[index2]->convert_to_index(get_pos_2(elem.first));
//                 auto parent = m_grid.children[index1]->children[index2]->children[index3];
//                 auto grid_index = parent->convert_to_index(get_pos_3(elem.first));
//                 auto grid = parent->children[grid_index];
//                 if(grid->size() == 0)
//                 {
//                     delete grid;
//                     parent->children[grid_index] = nullptr;
//                 }
//                 remove_queue.push_back({elem.first, elem.second - 1});
//                 break;
//             default:
//                 cl::log::fatal("Invalid remove index at " + __LINE__);
//             }
//         }
//     }

//     cl::Tile*& construct_grid_tile(cl::Vector2i position)
//     {
//         std::vector<cl::Vector2i> positions = get_positions(position);
//         auto one = m_grid.children[m_grid.convert_to_index(positions[0])];
//         if(one == nullptr)
//         {
//             one = new Grid<16, Grid<16, Grid<16, Grid<16, cl::Tile>>>>();
//         }
//         auto two = one->children[one->convert_to_index(positions[1])];
//         if(two = nullptr)
//         {
//             two = new Grid<16, Grid<16, Grid<16, cl::Tile>>>();
//         }
//         auto three = two->children[two->convert_to_index(positions[2])];
//         if(three = nullptr)
//         {
//             three = new Grid<16, Grid<16, cl::Tile>>();
//         }
//         auto four = three->children[three->convert_to_index(positions[3])];
//         if(four = nullptr)
//         {
//             four = new Grid<16, cl::Tile>();
//         }
//         return four->children[four->convert_to_index(positions[4])];
//     }

//     const cl::Tile* get_grid_tile(cl::Vector2i position)
//     {
//         std::vector<cl::Vector2i> positions = get_positions(position);
//         auto one = m_grid.children[m_grid.convert_to_index(positions[0])];
//         if(one == nullptr)
//         {
//             return nullptr;
//         }
//         auto two = one->children[one->convert_to_index(positions[1])];
//         if(two = nullptr)
//         {
//             return nullptr;
//         }
//         auto three = two->children[two->convert_to_index(positions[2])];
//         if(three = nullptr)
//         {
//             return nullptr;
//         }
//         auto four = three->children[three->convert_to_index(positions[3])];
//         if(four = nullptr)
//         {
//             return nullptr;
//         }
//         return four->children[four->convert_to_index(positions[4])];
//     }

//     // does not delete the grid tile
//     void remove_grid_tile(cl::Vector2i position)
//     {
//         std::vector<cl::Vector2i> positions = get_positions(position);
//         auto one = m_grid.children[m_grid.convert_to_index(positions[0])];
//         if(one == nullptr)
//         {
//             return;
//         }
//         auto two = one->children[one->convert_to_index(positions[1])];
//         if(two = nullptr)
//         {
//             return;
//         }
//         auto three = two->children[two->convert_to_index(positions[2])];
//         if(three = nullptr)
//         {
//             return;
//         }
//         auto four = three->children[three->convert_to_index(positions[3])];
//         if(four = nullptr)
//         {
//             return;
//         }
//         four->children[four->convert_to_index(positions[4])] = nullptr;
//     }

// private:
//     Grid<1, Grid<16, Grid<16, Grid<16, Grid<16, cl::Tile>>>>> m_grid;
//     std::list<std::pair<cl::Vector2i, uint8_t>> remove_queue;

//     cl::Vector2i get_pos_0(cl::Vector2i world_pos)
//     {
//         world_pos += VEC_GRID_HALF_SIZE;
//         return world_pos / GRID_HALF_SIZE;
//     }

//     cl::Vector2i get_pos_1(cl::Vector2i world_pos)
//     {
//         world_pos += VEC_GRID_HALF_SIZE - (get_pos_0(world_pos) * GRID_HALF_SIZE);
//         return world_pos / 32768;
//     }

//     cl::Vector2i get_pos_2(cl::Vector2i world_pos)
//     {
//         world_pos += VEC_GRID_HALF_SIZE - (get_pos_0(world_pos) * GRID_HALF_SIZE);
//         cl::Vector2i off = get_pos_1(world_pos) * 32768;
//         return (world_pos - off) / 1024;
//     }

//     cl::Vector2i get_pos_3(cl::Vector2i world_pos)
//     {
//         world_pos += VEC_GRID_HALF_SIZE - (get_pos_0(world_pos) * GRID_HALF_SIZE);
//         cl::Vector2i off = (get_pos_1(world_pos * 32768)) + (get_pos_2(world_pos) * 1024);
//         return (world_pos - off) / 32;
//     }

//     cl::Vector2i get_pos_4(cl::Vector2i world_pos)
//     {
//         world_pos += VEC_GRID_HALF_SIZE - (get_pos_0(world_pos) * GRID_HALF_SIZE);
//         cl::Vector2i off = (get_pos_1(world_pos * 32768)) + (get_pos_2(world_pos) * 1024) + (get_pos_3(world_pos) * 32);
//         return world_pos - off;
//     }

//     std::vector<cl::Vector2i> get_positions(const cl::Vector2i& world_pos)
//     {
//         const cl::Vector2i pos_world_pos = world_pos + (VEC_GRID_HALF_SIZE - (get_pos_0(world_pos) * GRID_HALF_SIZE));
//         std::vector<cl::Vector2i> positions;
//         cl::Vector2i temp = world_pos + VEC_GRID_HALF_SIZE;
//         positions.push_back(temp / GRID_HALF_SIZE);
//         positions.push_back((pos_world_pos) / 32768);
//         positions.push_back((pos_world_pos - (positions[1] * 32768)) / 1024);
//         positions.push_back((pos_world_pos - ((positions[1] * 32768) + (positions[2] * 1024))) / 32);
//         positions.push_back((pos_world_pos - ((positions[1] * 32768) + (positions[2] * 1024) + (positions[3] * 32))));
//         return positions;
//     }
// };

// // typedef cl::Vector2<cl::Vector2f> Rect;

// class QuadTree
// {
// public:

// };

// class DynamicQuadTree : public StorageBase
// {
// public:
//     DynamicQuadTree(const Rect& size, const int depth)
//     {
//         m_area = size;
//         m_depth = depth;
//         set_size(m_area);
//     }

//     DynamicQuadTree(const Rect& size)
//     {
//         m_area = size;
//         set_size(m_area);
//     }

//     DynamicQuadTree()
//     {
//         m_area = {{0, 0}, {GRID_HALF_SIZE, GRID_HALF_SIZE}};
//         set_size(m_area);
//     }

//     StorageType get_type() override
//     {
//         return DYNAMIC;
//     }

//     void insert(void* object) override
//     {
//         cl::GameObject* gameObject = (cl::GameObject*)object;
//         Rect size;
//         size.position = gameObject->Transform().position;
//         size.size = gameObject->Transform().scale;
//         for(int i = 0; i < 4; i++)
//         {
//             if(m_child_areas[i].contains(size))
//             {
//                 if(m_depth + 1 < MAX_DEPTH)
//                 {
//                     if(!m_children[i]) m_children[i] = new DynamicQuadTree(m_child_areas[i], m_depth + 1);
//                     m_children[i]->_insert(gameObject, size);
//                     return;
//                 }
//             }
//         }
//         auto p = new std::pair<Rect, cl::GameObject*>(size, gameObject);
//         m_storage.push_back(p);
//     }

//     void _insert(cl::GameObject* gameObject, const Rect& size)
//     {
//         for(int i = 0; i < 4; i++)
//         {
//             if(m_child_areas[i].contains(size))
//             {
//                 if(m_depth + 1 < MAX_DEPTH)
//                 {
//                     if(!m_children[i]) m_children[i] = new DynamicQuadTree(m_child_areas[i], m_depth + 1);
//                     m_children[i]->_insert(gameObject, size);
//                     return;
//                 }
//             }
//         }
//         auto p = new std::pair<Rect, cl::GameObject*>(size, gameObject);
//         m_storage.push_back(p);
//     }

//     std::list<void*> search(cl::Vector2d position, cl::Vector2d half_width) override
//     {
//         std::list<void*> items;
//         Rect area = {position, half_width};
//         search(area, items);
//         return items;
//     }

//     void search(const Rect& area, std::list<void*>& items)
//     {
//         for(const void* item : m_storage)
//         {
//             auto i = (std::pair<Rect, cl::GameObject*>*)item;
//             if(area.overlaps(i->first)) items.push_back(i->second);
//         }

//         for(int i = 0; i < 4; i++)
//         {
//             if(m_children[i])
//             {
//                 if(area.contains(m_child_areas[i])) m_children[i]->items(items);
//                 else if(m_child_areas[i].overlaps(area)) m_children[i]->search(area, items);
//             }
//         }
//     }

//     void items(std::list<void*>& items)
//     {
//         for(const void* item : m_storage) items.push_back(((std::pair<Rect, cl::GameObject*>*)item)->second);
//         for(int i = 0; i < 4; i++) if(m_children[i]) m_children[i]->items(items);
//     }

//     void remove(void* object) override
//     {
//     }

//     void remove(cl::Vector2d position, cl::Vector2d half_width) override
//     {
//     }

//     void clear() override
//     {
//         for(int i = 0; i < 4; i++)
//         {
//             if(m_children[i])
//             {
//                 m_children[i]->clear();
//                 delete m_children[i];
//                 m_children[i] = nullptr;
//             }
//         }
//         m_storage.clear();
//     }

//     void update() override
//     {
//     }

// private:
//     // DynamicQuadTree* m_children[4] = {nullptr};
//     Rect m_child_areas[4] = {{}};
//     Rect m_area = {};
//     int m_depth = 0;

//     void set_size(const Rect& size)
//     {
//         cl::Vector2d child_size = size.size * 0.5f;
//         m_child_areas[0] = Rect(size.position, child_size);
//         m_child_areas[1] = Rect({size.position.x + child_size.x, size.position.y}, child_size);
//         m_child_areas[2] = Rect({size.position.x, size.position.y + child_size.y}, child_size);
//         m_child_areas[3] = Rect(size.position + child_size, child_size);
//     }
// };

// StorageType get_type(void* storage)
// {
//     return ((StorageBase*)storage)->get_type();
// }

// StorageBase* get_base(void* storage)
// {
//     return (StorageBase*)storage;
// }

using namespace cl;
using namespace sa;


using QuadTreeContainer = std::list<QuadTreeItem<cl::GameObject>>;
using QuadTreeIterator = QuadTreeContainer::iterator;

#define MAX_DEPTH 20

class QuadTree
{
public:
    QuadTree(const Rect& size = {{0.0f, 0.0f}, {100.0f, 100.0f}}, const size_t depth = 0)
    {
        m_depth = depth;
        resize(size);
    }

    void resize(const Rect& area)
    {
        clear();
        m_area = area;
        m_child_areas[0] = {area.position, area.size * 0.5};
        m_child_areas[1] = {area.position + cl::Vector2d(area.size.x * 0.5, 0), area.size * 0.5};
        m_child_areas[2] = {area.position + cl::Vector2d(0, area.size.y * 0.5), area.size * 0.5};
        m_child_areas[3] = {area.position + (area.size * 0.5), area.size * 0.5};
    }

    void clear()
    {
        while(m_items.size() > 0)
        {
            delete m_items.front().first->item;
            m_items.pop_front();
        }
        for(int i = 0; i < 4; i++)
        {
            if(m_children[i])
            {
                m_children[i]->clear();
                delete m_children[i];
                m_children[i] = nullptr;
            }
        }
    }

    size_t size() const
    {
        size_t count = m_items.size();
        for(int i = 0; i < 4; i++)
        {
            if(m_children[i]) count += m_children[i]->size();
        }
        return count;
    }

    QuadTreeItemLocation<QuadTreeIterator> insert(const QuadTreeIterator& item, const Rect& item_size)
    {
        for(int i = 0; i < 4; i++)
        {
            if(m_child_areas[i].contains(item_size))
            {
                if(m_depth + 1 < MAX_DEPTH)
                {
                    if(!m_children[i]) m_children[i] = new QuadTree(m_child_areas[i], m_depth + 1);
                    return m_children[i]->insert(item, item_size);
                }
            }
        }

        m_items.push_back({item, item_size});
        return {&m_items, std::prev(m_items.end())};
    }

    std::list<ItemIterator> search(const Rect& area) const
    {
        std::list<ItemIterator> items;
        search(area, items);
        return items;
    }

    void search(const Rect& area, std::list<ItemIterator>& items) const
    {
        for(const auto& item : m_items)
        {
            if(area.overlaps(item.second))
            {
                items.push_back(item.first);
            }
        }

        for(int i = 0; i < 4; i++)
        {
            if(m_children[i])
            {
                if(area.contains(m_child_areas[i])) m_children[i]->items(items);
                else if(m_child_areas[i].overlaps(area)) m_children[i]->search(area, items);
            }
        }
    }

    void items(std::list<ItemIterator>& items) const
    {
        for(const auto& item : m_items)
        {
            items.push_back(item.first);
        }

        for(int i = 0; i < 4; i++)
        {
            if(m_children[i]) m_children[i]->items(items);
        }
    }

    const Rect& area()
    {
        return m_area;
    }

protected:
    size_t m_depth = 0;
    Rect m_area;
    Rect m_child_areas[4];
    QuadTree* m_children[4] = {nullptr};
    std::list<std::pair<QuadTreeIterator, Rect>> m_items;
};

class QuadTreeWrapper
{

protected:
    QuadTreeContainer m_all_items;
    QuadTree root;

public:
    QuadTreeWrapper(const Rect& size = {{0.0f, 0.0f}, {100.0f, 100.0f}}) : root(size)
    {

    }

    void resize(const Rect& area)
    {
        root.resize(area);
    }

    size_t size() const
    {
        return m_all_items.size();
    }

    bool empty() const
    {
        return m_all_items.empty();
    }

    void clear()
    {
        root.clear();
        m_all_items.clear();
    }

    QuadTreeIterator begin()
    {
        return m_all_items.begin();
    }

    QuadTreeIterator end()
    {
        return m_all_items.end();
    }

    typename QuadTreeContainer::const_iterator cbegin()
    {
        return m_all_items.cbegin();
    }

    typename QuadTreeContainer::const_iterator cend()
    {
        return m_all_items.cend();
    }

    void insert(const cl::GameObject& item)
    {
        QuadTreeItem<cl::GameObject> qt_item;
        qt_item.item = (cl::GameObject*)&item;
        m_all_items.push_back(qt_item);
        const Rect size = {item.Transform().position, item.Transform().scale};
        m_all_items.back().location = root.insert(std::prev(m_all_items.end()), size);
    }

    std::list<ItemIterator> search(const Rect& area) const
    {
        return root.search(area);
    }

    void remove(QuadTreeIterator item)
    {
        delete item->item;
        item->location.container->erase(item->location.iterator);
        m_all_items.erase(item);
    }

    void relocate(QuadTreeIterator& item)
    {
        const Rect size = {item->item->Transform().position, item->item->Transform().scale};
        item->location.container->erase(item->location.iterator);
        item->location = root.insert(item, size);
    }
};

extern "C"
{
    void init(LibraryManager* libraryManager)
    {
        cl::log::detail::load_lib(libraryManager);
    }

    void* create_storage(const cl::Vector2d& position, const cl::Vector2d& size)
    {
        return new QuadTreeWrapper(Rect(position, size));
    }

    void resize(void* container, const cl::Vector2d& position, const cl::Vector2d& size)
    {
        ((QuadTreeWrapper*)container)->resize(Rect(position, size));
    }

    size_t size(void* container)
    {
        return ((QuadTreeWrapper*)container)->size();
    }

    bool empty(void* container)
    {
        return ((QuadTreeWrapper*)container)->empty();
    }

    void clear(void* container)
    {
        ((QuadTreeWrapper*)container)->clear();
    }

    ItemIterator begin(void* container)
    {
        return ((QuadTreeWrapper*)container)->begin();
    }

    ItemIterator end(void* container)
    {
        return ((QuadTreeWrapper*)container)->end();
    }

    ConstItemIterator cbegin(void* container)
    {
        return ((QuadTreeWrapper*)container)->cbegin();
    }

    ConstItemIterator cend(void* container)
    {
        return ((QuadTreeWrapper*)container)->cend();
    }

    void insert(void* container, const cl::GameObject& item)
    {
        ((QuadTreeWrapper*)container)->insert(item);
    }

    std::list<ItemIterator> search(void* container, const cl::Vector2d& position, const cl::Vector2d& size)
    {
        return ((QuadTreeWrapper*)container)->search({position, size});
    }

    void erase(void* container, ItemIterator item)
    {
        ((QuadTreeWrapper*)container)->remove(item);
    }

    void relocate(void* container, ItemIterator item)
    {
        ((QuadTreeWrapper*)container)->relocate(item);
    }
}