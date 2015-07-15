#ifndef MIO_DEQUE_H_INCLUDED
#define MIO_DEQUE_H_INCLUDED

#include "mio_alloc.h"
#include <iterator>
#include <bits/stl_construct.h>

namespace mio
{

// 计算缓冲区大小(元素个数)
inline size_t __deque_buf_size(size_t n, size_t sz)
{
    return n ? n : (sz < 512 ? static_cast<size_t>(512 / sz) : size_t(1));
}

/**
 *  deque迭代器
 */
template <class T, class Ref, class Ptr, size_t BufSize>
class __deque_iterator
{
public:
    typedef __deque_iterator<T, T&, T*, BufSize> iterator;
    typedef __deque_iterator<T, const T&, const T*, BufSize> const_iterator;

    static size_t buffer_size()
    {
        return __deque_buf_size(BufSize, sizeof(T));
    }

    typedef std::random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T** map_pointer;  // 主控
    typedef __deque_iterator self;

    // 保持与容器的联结
    T *cur;    // 此迭代器所指之缓冲区的当前元素
    T *first;  // 此迭代器所指之缓冲区的头
    T *last;   // 此迭代器所指之缓冲区的尾
    map_pointer node;  // 指向管控中心

    // 改变当前迭代器指向的缓冲区
    void set_node(map_pointer new_node)
    {
        node = new_node;
        first = *new_node;
        last = first + static_cast<difference_type>(buffer_size());
    }

    reference operator*() const
    {
        return *cur;
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    // 当前迭代器位于x之后
    difference_type operator-(const self &x) const
    {
        return static_cast<difference_type>(buffer_size()) * (node - x.node - 1) +
               (x.last - x.cur) + (cur - first);
    }

    self& operator++()
    {
        if(++cur == last)  // 向后移动，如果到达缓冲区尾端，切换至下一个缓冲区
        {
            set_node(node + 1);
            cur = first;
        }

        return *this;
    }

    self& operator++(int)
    {
        self temp = *this;

        ++*this;

        return temp;
    }

    self& operator--()
    {
        if(cur == first)
        {
            set_node(node - 1);
            cur = last;
        }

        --cur;

        return *this;
    }

    self operator--(int)
    {
        self temp = *this;

        --this;

        return temp;
    }

    // 实现随机存取
    self& operator+=(difference_type n)
    {
        int offset = (cur - first) + n;

        if(offset >= 0 && offset < static_cast<difference_type>(buffer_size()))
        {
            // 目标位置在同一缓冲区中

            cur += n;
        }
        else
        {
            // 目标位置在不同缓冲区中

            difference_type node_offset = offset > 0 ?
                            offset / static_cast<difference_type>(buffer_size()) :
                            (offset + 1) / static_cast<difference_type>(buffer_size()) - 1;

            set_node(node + offset);
            cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size()));
        }

        return *this;
    }

    self operator+(difference_type n) const
    {
        self temp = *this;

        return temp += n;
    }

    self& operator-=(difference_type n)
    {
        return *this += -n;
    }

    self operator-(difference_type n)
    {
        self temp = *this;

        return temp -= n;
    }

    reference operator[](difference_type n) const
    {
        return *(*this + n);
    }

    bool operator==(const self &x) const
    {
        return cur == x.cur;
    }

    bool operator!=(const self &x) const
    {
        return !(*this == x);
    }

    bool operator<(const self &x) const
    {
        return node == x.node ? (cur < x.cur) : (node < x.node);
    }
};

typedef default_alloc alloc;

/**
 *  双端队列deque
 */
template <class T, class Alloc = alloc, size_t BufSize = 0>
class deque
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef T& reference;
    typedef __deque_iterator<T, T&, T*, BufSize> iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

protected:
    typedef pointer* map_pointer;  // 元素指针的指针
    // 专属空间配置器，每次配置一个元素大小
    typedef simple_alloc<value_type, Alloc> data_allocator;
    // 专属空间配置器，每次配置一个指针大小
    typedef simple_alloc<pointer, Alloc> map_allocator;

protected:
    iterator start;      // 指向第一个节点
    iterator finish;     // 指向最后一个节点
    map_pointer map;     // 指向map的指针
    size_type mapSize;  // map内部指针数量

public:
    iterator begin()
    {
        return start;
    }

    iterator end()
    {
        return finish;
    }

    reference operator[](size_type n)
    {
        return start[static_cast<difference_type>(n)];
    }

    reference front()
    {
        return *start;
    }

    reference back()
    {
        return *(finish - 1);
    }

    size_type size() const
    {
        return finish - start;
    }

    size_type max_size() const
    {
        return static_cast<size_type>(-1);  // size_type为无符号数
    }

    bool empty()
    {
        return finish == start;
    }

protected:
    size_type buffer_size()
    {
        return iterator::buffer_size();
    }

    pointer allocate_node()
    {
        return data_allocator::allocate(buffer_size() *
                                        sizeof(value_type));
    }

    void deallocate_node(pointer p)
    {
        data_allocator::deallocate(p, buffer_size() *
                                   sizeof(value_type));
    }

    // 创建含有num_elements个节点的map
    void creat_map_and_nodes(size_type num_elements);

    // 构建deque并赋初值
    void fill_initialize(size_type n, const value_type &value);

private:
    // 当map尾部空间不足时调用
    void reserve_map_at_back(size_type nodesToAdd = 1)
    {
        if(nodesToAdd + 1 > mapSize - (finish.node - map))
        {
            // map尾部的备用节点空间不足，需要更换map
            // 当尾部满时，map_size - (finish.node - map)值应当为1
            reallocate_map(nodesToAdd, false);
        }
    }

    // 当map头部空间不足时调用
    void reserve_map_at_front(size_type nodesToAdd = 1)
    {
        if(nodesToAdd > start.node - map)
        {
            // map头部的备用节点空间不足，需要更换map
            reallocate_map(nodesToAdd, true);
        }
    }

    // 当map空间不足时，配置新的map
    void reallocate_map(size_type nodesToAdd, bool addAtFront);

    // 当最后一个缓冲区只剩一个元素备用空间时调用
    void push_back_aux(const value_type &t);

    // 当第一个缓冲区没有备用空间时调用
    void push_front_aux(const value_type &t);

    // 当最后一个缓冲区为空时调用
    void pop_back_aux();

    // 当第一个缓冲区只剩一个元素时调用
    void pop_front_aux();

public:
    // constructor
    deque(int n, const value_type &value)
    :start(), finish(), map(nullptr), mapSize(0)
    {
        fill_initialize(n, value);
    }

    void push_back(const value_type &t)
    {
        if(finish.cur != finish.last - 1)
        {
            // 如果缓冲区有两个（含）以上元素备用空间，
            // 直接在备用空间上构造元素
            std::_Construct(finish.cur, t);
            ++finish.cur;
        }
        else
        {
            // 如果缓冲区只剩最后一个元素备用空间
            push_back_aux(t);
        }
    }

    void push_front(const value_type &t)
    {
        if(start.cur != start.first)
        {
            // 第一缓冲区有备用空间
            std::_Construct(start.cur - 1, t);
            --start.cur;
        }
        else
        {
            // 第一缓冲区已无备用空间
            push_front_aux();
        }
    }

    void pop_back()
    {
        if(finish.cur != finish.first)
        {
            // 最后一个缓冲区中含有一个或多个元素
            --finish.cur;
            std::_Destroy(finish.cur);
        }
        else
        {
            pop_back_aux();
        }
    }

    void pop_front()
    {
        if(start.cur != start.last - 1)
        {
            // 第一个缓冲区中含有一个以上元素
            std::_Destroy(start.cur);
            ++start.cur;
        }
        else
        {
            pop_front_aux();
        }
    }
};

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::creat_map_and_nodes(size_type num_elements)
{
    // 需要节点数 = (元素个数/缓冲区大小) + 1
    // 刚好整除时多一个节点
    size_type num_nodes = num_elements / buffer_size() + 1;

    // 一个map管理多个节点。最少8个,最多为所需节点数+2(前后各预留一个)
    mapSize = std::max(static_cast<size_type>(8), num_nodes + 2);
    // 配置具有map_size个节点的map
    map = map_allocator::allocate(mapSize);

    // 令nstart和nfinish指向map的中间节点，保证头尾的可扩充空间一样大
    map_pointer nstart = map + (mapSize - num_nodes) / 2;
    map_pointer nfinish = nstart + num_nodes - 1;

    // 为每个要使用的节点配置缓冲区template <class T, class Alloc, size_t BufSize>
    map_pointer cur;

    try
    {
        for(cur = nstart; cur <= nfinish; ++cur)
        {
            *cur = allocate_node();
        }
    }
    catch(...)
    {
        for(cur = nstart; cur <= nfinish; ++cur)
        {
            if(cur != nullptr)
            {
                deallocate_node(*cur);
            }
        }

        throw;
    }

    // 为内部迭代器start和end设定正确内容
    start.set_node(nstart);
    finish.set_node(nfinish);
    start.cur = start.first;
    // 对于刚好整除多分配一个节点的情况，令cur指向多分配的节点的起始处
    finish.cur = finish.first + num_elements % buffer_size();
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::fill_initialize(size_type n, const value_type &value)
{
    creat_map_and_nodes(n);

    map_pointer cur;

    try
    {
        // 为每个节点的缓冲区设置初值
        for(cur = start.node; cur < finish.node; ++cur)
        {
            std::uninitialized_fill(*cur, *cur + buffer_size(), value);
        }
        // 尾端可能有备用空间，不必设定初值(由于finish节点可能指向多分配的节点)
        std::uninitialized_fill(finish.first, finish.cur, value);
    }
    catch(...)
    {
        // 为每个节点的缓冲区设置初值
        for(cur = start.node; cur < finish.node; ++cur)
        {
            std::_Destroy(*cur, *cur + buffer_size());
        }
        // 尾端可能有备用空间
        std::_Destroy(finish.first, finish.cur);

        throw;
    }
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::reallocate_map(size_type nodesToAdd, bool addAtFront)
{
    size_type oldNumNodes = finish.node - start.node + 1;
    size_type newNumNodes = oldNumNodes + nodesToAdd;
    map_pointer newStart;

    if(mapSize > 2 * newNumNodes)
    {
        newStart = map + (mapSize - newNumNodes) / 2 +
                   (addAtFront ? nodesToAdd : 0);

        if(newStart < start.node)
        {
            // 复制[x, y)到[z, ?)
            std::copy(start.node, finish.node + 1, newStart);
        }
        else
        {
            // 复制[x, y)到[?, z);
            std::copy_backward(start.node, finish.node + 1, newStart + oldNumNodes);
        }
    }
    else
    {
        // 空间不足，配置新map
        size_type newMapSize = mapSize + std::max(mapSize, nodesToAdd) + 2;
        map_pointer newMap = map_allocator::allocate(newMapSize);
        newStart = newMap + (newMapSize - newNumNodes) / 2 +
                   (addAtFront ? nodesToAdd : 0);

        // 将原来map中的内容复制到新的map中
        std::copy(start.node, finish.node + 1, newStart);
        // 释放原来的map;
        map_allocator::deallocate(map, mapSize);
        // 设定新的map的起始位置和大小
        map = newMap;
        mapSize = newMapSize;
    }

    // 重新设置start和finish
    start.set_node(newStart);
    finish.set_node(newStart + oldNumNodes - 1);
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_back_aux(const value_type &t)
{
    value_type tCopy = t;

    reserve_map_at_back();                 // map空间不足，需要扩充
    *(finish.node + 1) = allocate_node();  // 配置一个新的缓冲区

    try
    {
        std::_Construct(finish.cur, tCopy);  // 构造元素
        finish.set_node(finish.node + 1);    // 令finish指向新节点
        finish.cur = finish.first;           // 改变finish的状态
    }
    catch(...)
    {
        deallocate_node(*(finish.node + 1));
        throw;
    }
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_front_aux(const value_type &t)
{
    value_type tCopy = t;

    reserve_map_at_front();                // map空间不足，需要扩充
    *(start.node - 1) = allocate_node();  // 配置一个新的缓冲区

    try
    {
        start.set_node(start.node - 1);     // 令start指向新节点
        start.cur = start.last - 1;          // 改变start的状态
        std::_Construct(start.cur, tCopy);  // 构造元素
    }
    catch(...)
    {
        start.set_node(start.node + 1);
        start.cur = start.first;
        deallocate_node(*(start.node + 1));
        throw;
    }
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::pop_back_aux()
{
    deallocate_node(finish.first);     // 释放最后一个缓冲区
    finish.set_node(finish.node - 1);  // 调整finish节点，指向上一个缓冲区
    finish.cur = finish.last - 1;      // 指向上一个缓冲区的最后一个元素
    std::_Destroy(finish.cur);         // 析构该元素
}

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::pop_front_aux()
{
    std::_Destroy(start.cur);      // 析构第一个缓冲区的最后一个元素
    deallocate_node(start.first);  // 释放第一个缓冲区
    start.set_node(start + 1);     // 调整start节点，指向下一个缓冲区
    start.cur = start.first;       // 指向下一个缓冲区的第一个节点
}

}  // end of namespace mio

#endif // MIO_DEQUE_H_INCLUDED
