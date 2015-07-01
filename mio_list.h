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
        node = node->prev;

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

    // 将[first,last)内的所有原始移动到position之前
    void transfer(iterator position, iterator first, iterator last);

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

    // 依次打印链表值（仅测试使用）
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

    void swap(list &x)
    {
        std::swap(this->node, x.node);
    }

    // 清除所有节点
    void clear();

    // 删除所有值为value的元素
    void remove(const T &value);

    // 移除数值相同的连续元素
    void unique();

    // 将x接合于position之前的位置，x必须不同于*this
    void splice(iterator position, list &x)
    {
        if(!x.empty())
        {
            transfer(position, x.begin(), x.end());
        }
    }

    // 将i所指的元素接合于position之前
    void splice(iterator position, list&, iterator i)
    {
        iterator j = i;
        ++j;

        if(position == i || position == j)
        {
            return;
        }

        transfer(position, i, j);
    }

    // 将[first, last)内所有元素接合到position所指位置之前
    void splice(iterator position, iterator first, iterator last)
    {
        if(first != last)
        {
            transfer(position, first, last);
        }
    }

    // 合并2个排序好的链表,x到*this
    void merge(list<T, Alloc>& x);

    // 逆置链表
    void reverse();

    // 使用QuickSort对元素进行排序
    void sort();
};

template <class  T, class Alloc>
void list<T, Alloc>::transfer(iterator position, iterator first, iterator last)
{
    if(position != last)
    {
        last.node->prev->next = position.node;
        first.node->prev->next = last.node;
        position.node->prev->next = first.node;
        link_type temp = position.node->prev;
        position.node->prev = last.node->prev;
        last.node->prev = first.node->prev;
        first.node->prev = temp;
    }
}

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

template <class T, class Alloc>
void list<T, Alloc>::merge(list<T, Alloc>& x)
{
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = x.begin();
    iterator last2 = x.end();

    // 将x的元素插入到*this中
    while(first1 != last1 && first2 != last2)
    {
        if(*first1 > *first2)
        {
            iterator next = first2;
            transfer(first1, first2, ++next);
            first2 = next;
        }

        ++first1;
    }

    // 将剩余x元素全部置于*this尾部
    if(first2 != last2)
    {
        transfer(last1, first2, last2);
    }
}

template <class T, class Alloc>
void list<T, Alloc>::reverse()
{
    // 如果链表只含有0个或1个元素则无需处理
    if(node->next == node || node->next->next == node)
    {
        return;
    }

    // 从前往后依次将元素插入到node之后，得到逆序链表
    iterator first = begin();
    ++first;

    while(first != end())
    {
        iterator old = first;
        ++first;
        transfer(begin(), old, first);
    }
}

template <class T, class Alloc>
void list<T, Alloc>::sort()
{
}

}  // end of namespace mio

#endif // MIO_LIST_H_INCLUDED
