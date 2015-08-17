#ifndef MIO_PRIORITY_QUEUE_H_INCLUDED
#define MIO_PRIORITY_QUEUE_H_INCLUDED

#include <functional>

#include "mio_vector.h"
#include "mio_heap.h"

namespace mio
{

template <class T, class Container = vector<T>,
          class Compare = std::less<typename Container::value_type>>
class priority_queue
{
public:
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

public:
    Container c;
    Compare comp;  // 比较函数

public:
    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last)
    :c(first, last)
    {
        make_heap(c.begin(), c.end());
    }

    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last, const Compare& x)
    :c(first, last), comp(x)
    {
        make_heap(c.begin(), c.end());
    }

    bool empty() const
    {
        return c.empty();
    }

    size_type size() const
    {
        return c.size();
    }

    const_reference top() const
    {
        return c.front();
    }

    void push(const value_type& x)
    {
        try
        {
            c.push_back(x);
            push_heap(c.begin(), c.end());
        }
        catch(...)
        {
            c.clear();
        }
    }

    void pop()
    {
        try
        {
            pop_heap(c.begin(), c.end());
            c.pop_back();
        }
        catch(...)
        {
            c.clear();
        }
    }
};

}  // end of namespace mio

#endif // MIO_PRIORITY_QUEUE_H_INCLUDED
