#ifndef MIO_LIST_H_INCLUDED
#define MIO_LIST_H_INCLUDED

#include "mio_alloc.h"
#include <iterator>

namespace mio
{

/**
 *  list�ڵ�
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
 *  list������
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

    link_type node;  // ָ��list�Ľڵ�

    // construct
    __list_iterator(link_type x): node(x) {}

    __list_iterator() {}

    //iterator__list_iterator(const iterator &x): node(x) {}

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
 *  ѭ��˫������
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
    link_type node;  // nodeָ��ĩβ�Ŀհ׽ڵ�
    typedef simple_alloc<value_type, Alloc> data_allocator;  // �ռ�������

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
        node = data_allocator::allocate();
        node->prev = node;
        node->next = node;
    }

    void push_back(const T &x)
    {
        link_type newNode = data_allocator::allocate();
        newNode->data = x;

        link_type temp = node->prev;
        node->prev = newNode;
        newNode->next = node;
        newNode->prev = temp;
        temp->next = newNode;
    }
};

}  // end of namespace mio

#endif // MIO_LIST_H_INCLUDED
