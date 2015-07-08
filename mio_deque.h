#ifndef MIO_DEQUE_H_INCLUDED
#define MIO_DEQUE_H_INCLUDED

#include "mio_alloc.h"
#include <iterator>
#include <bits/stl_construct.h>

namespace mio
{

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
    size_type map_size;  // map内部指针数量

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
    void creat_map_and_nodes(size_type num_elements)
    {
        // 需要节点数 = (元素个数/缓冲区大小) + 1
        // 刚好整除时多一个节点
        size_type num_nodes = num_elements / buffer_size() + 1;

        //
    }
};
}  // end of namespace mio



#endif // MIO_DEQUE_H_INCLUDED
