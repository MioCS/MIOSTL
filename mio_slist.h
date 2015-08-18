#ifndef MIO_SLIST_H_INCLUDED
#define MIO_SLIST_H_INCLUDED

/// 未完成
#include <iterator>
#include <bits/stl_construct.h>

#include "mio_alloc.h"

namespace mio
{

// 单向链表的节点基本结构
struct __slist_node_base
{
    __slist_node_base* next;
};

// 单向链表的节点结构
template <class T>
struct __slist_node : public __slist_node_base
{
    T data;
};

// 已知某一节点，插入新节点于其后
inline __slist_node_base* __slist_make_link(__slist_node_base* prevNode,
                                            __slist_node_base* newNode)
{
    newNode->next = prevNode->next;
    prevNode->next = newNode;

    return newNode;
}

// 获得单向链表的元素个数
inline size_t __slist_size(__slist_node_base* node)
{
    size_t result = 0;
    __slist_node_base* cur = node;

    while(node)
    {
        ++result;
        cur = cur->next;
    }

    return result;
}

// 单向链表迭代器基本结构
struct __slist_iterator_base
{
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    __slist_node_base* node; // 指向节点基本结构

    __slist_iterator_base(__slist_node_base* x)
    :node(x)
    {
    }

    // 前进一个节点
    void incr()
    {
        node = node->next;
    }

    bool operator==(const __slist_iterator_base& x) const
    {
        return node == x.node;
    }

    bool operator!=(const __slist_iterator_base& x) const
    {
        return node != x.node;
    }
};

template <class T, class Ref, class Ptr>
struct __slist_iterator : public __slist_iterator_base
{
    typedef __slist_iterator<T, T&, T*> iterator;
    typedef __slist_iterator<T, const T&, const T*> const_iterator;
    typedef __slist_iterator<T, Ref, Ptr> self;

    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __slist_node<T> list_node;

    // 构造函数
    __slist_iterator(list_node* x)
    :__slist_iterator_base(x)
    {
    }

    __slist_iterator()
    :__slist_iterator_base(nullptr)
    {
    }

    __slist_iterator(const __slist_iterator& x)
    :__slist_iterator_base(x.node)
    {
    }

    reference operator*() const
    {
        return ((list_node *)node)->data;
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    self& operator++()
    {
        incr();

        return *this;
    }

    self& operator++(int)
    {
        self temp = *this;
        incr();

        return temp;
    }
};

}  // end of namespace mio

#endif // MIO_SLIST_H_INCLUDED
