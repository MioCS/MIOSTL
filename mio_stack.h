#ifndef MIO_STACK_H_INCLUDED
#define MIO_STACK_H_INCLUDED

#include "mio_deque.h"

namespace mio
{

template <class T, class Container = deque<T>>
class stack;

template <class T, class Container>
bool operator==(const stack<T, Container> &x, const stack<T, Container> &y)
{
    return x.c == y.c;
}

template <class T, class Container>
bool operator<(const stack<T, Container> &x, const stack<T, Container> &y)
{
    return x.c < y.c;
}

template <class T, class Container>
class stack
{
friend bool operator==<T, Container>(const stack<T, Container> &x,
                                     const stack<T, Container> &y);
friend bool operator< <T, Container>(const stack<T, Container> &x,
                                     const stack<T, Container> &y);

public:
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

protected:
    Container c;

public:
    stack(size_type n, const value_type &value)
    :c(n, value)
    {
    }

    bool empty() const
    {
        return c.empty();
    }

    size_type size()
    {
        return c.size();
    }

    reference top()
    {
        return c.back();
    }

    const_reference top() const
    {
        return c.back();
    }

    void push(const value_type &x)
    {
        c.push_back(x);
    }

    void pop()
    {
        c.pop_back();
    }
};

}  // end of namespace mio




#endif // MIO_STACK_H_INCLUDED
