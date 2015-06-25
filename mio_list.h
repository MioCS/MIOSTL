#ifndef MIO_LIST_H_INCLUDED
#define MIO_LIST_H_INCLUDED

#include "mio_alloc.h"
#include <iterator>
#include <bits/stl_construct.h>

namespace mio
{

/**
 *  list节点
 */
template <class T>
class __list_node
{
public:
    __list_node<T> *prev;
    __list_node<T> *next;
    T data;
};

/**
 *  list迭代器
 */
template <class T, class Ref, class Ptr>
class __list_iterator
{
public:
    typedef __list_iterator<T, T&, T*> iterator;
    typedef __list_iterator<T, Ref, Ptr> self;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef __list_node<T>* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    link_type node;  // 指向list的节点

    // construct
    __list_iterator(link_type x): node(x) {}

    __list_iterator() {}

    __list_iterator(const iterator &x): node(x.node) {}

    bool operator==(const self& x) const
    {
        return node == x.node;
    }

    bool operator!=(const self& x) const
    {
        return node != x.node;
    }

    // dereference
    reference operator*()
    {
        return (*node).data;
    }

    pointer operator->()
    {
        return &(operator*());
    }

    self &operator++()
    {
        node = node->next;

        return *this;
    }

    self operator++(int)
    {
        self temp = *this;
        ++*this;

        return temp;
    }

    self &operator--()
    {
        node = node->pre;

        return *this;
    }

    self operator--(int)
    {
        self temp = *this;
        --*this;

        return *this;
    }
};

typedef default_alloc alloc;

/**
 *  循环双向链表
 */
template <class T, class Alloc = alloc>
class list
{
protected:
    typedef __list_node<T> list_node;

public:
    typedef list_node* link_type;
    typedef __list_node<T> value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef __list_iterator<T, T&, T*> iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

protected:
    link_type node;  // node指向末尾的空白节点
    typedef simple_alloc<value_type, Alloc> data_allocator;  // 空间配置器

    // 分配一节点空间
    link_type allocate()
    {
        return data_allocator::allocate();
    }

    // 释放一个节点空间
    void deallocate(link_type p)
    {
        data_allocator::deallocate(p);
    }

    // 配置并构造一个节点
    link_type create_node(const T &x)
    {
        link_type p = allocate();
        std::_Construct(&p->data, x);

        return p;
    }

    // 析构并释放一个节点
    void destory_node(link_type p)
    {
        std::_Destroy(&p->data);
        deallocate(p);
    }

    void empty_initialize()
    {
        node = allocate();
        node->next = node;
        node->prev = node;
    }

public:
    iterator begin()
    {
        return iterator(node->next);
    }

    iterator end()
    {
        return iterator(node);
    }

    bool empty() const
    {
        return node->next == node;
    }

    size_type size() const
    {
        size_type result = 0;
        link_type temp = node;

        while(temp->next != node)
        {
            ++result;
            temp = temp->next;
        }

        return result;
    }

    reference front()
    {
        return *begin();
    }

    reference back()
    {
        return *(--end());
    }

    // construct
    list()
    {
        empty_initialize();
    }

    // 在position处插入值为x的节点
    iterator insert(iterator position, const T &x)
    {
        // 生成新节点
        link_type temp = create_node(x);

        // 插入新节点
        temp->prev = position.node->prev;
        temp->next = position.node;
        position.node->prev->next = temp;
        position.node->prev = temp;

        return iterator(temp);
    }

    void push_front(const T &x)
    {
        insert(begin(), x);
    }

    void push_back(const T &x)
    {
        insert(end(), x);
    }

    // 删除position位置的节点，并返回其后位置节点的迭代器
    iterator erase(iterator position)
    {
        link_type posNode = position.node;
        link_type posNext = posNode->next;

        posNode->prev->next = posNode->next;
        posNode->next->prev = posNode->prev;

        destory_node(posNode);

        return posNext;
    }

    void pop_front()
    {
        erase(begin());
    }

    void pop_back()
    {
        iterator temp = end();
        erase(--end());
    }

    void show()
    {
        iterator cur = begin();

        while(cur != end())
        {
            std::cout << *cur << std::ends;
            ++cur;
        }
        std::cout << std::endl;
    }

    // 清除所有节点
    void clear();

    // 删除所有值为value的元素
    void remove(const T &value);

    // 移除数值相同的连续元素
    void unique();
};

template <class T, class Alloc>
void list<T, Alloc>::clear()
{
    link_type cur = node->next;

    while(cur != node)
    {
        link_type curNext = cur->next;
        destory_node(cur);
        cur = curNext;
    }

    // 恢复node的原始状态
    node->next = node;
    node->prev = node;
}

template <class T, class Alloc>
void list<T, Alloc>::remove(const T &value)
{
    iterator first = begin();
    iterator last = end();

    while(first != last)
    {
        iterator next = first;
        ++next;

        if(*first == value)
        {
            erase(first);
        }

        first = next;
    }
}

template <class T, class Alloc>
void list<T, Alloc>::unique()
{
    iterator first = begin();
    iterator last = end();

    // 空链表不处理
    if(first == last)
    {
        return;
    }

    iterator next = first;

    while(++next != last)
    {
        if(*next == *first)
        {
            erase(next);
            next = first;
        }
        else
        {
            ++first;
        }
    }
}

}  // end of namespace mio

#endif // MIO_LIST_H_INCLUDED
