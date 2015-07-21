#ifndef MIO_QUEUE_H_INCLUDED
#define MIO_QUEUE_H_INCLUDED

namespace mio
{

template <class T, class Container = deque<T>>
class queue;

template <class T, class Container>
bool operator==(const queue<T, Container> &x, const queue<T, Container> &y)
{
    return x.c == y.c;
}

template <class T, class Container>
bool operator<(const queue<T, Container> &x, const queue<T, Container> &y)
{
    return x.c < y.c;
}

template <class T, class Container>
class queue
{
friend bool operator==<T, Container>(const queue<T, Container> &x,
                                     const queue<T, Container> &y);
friend bool operator< <T, Container>(const queue<T, Container> &x,
                                     const queue<T, Container> &y);

public:
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

protected:
    Container c;

public:
    queue(size_type n, const value_type &value)
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
        c.push_front(x);
    }

    void pop()
    {
        c.pop_front();
    }
};

}  // end of namespace mio

#endif // MIO_QUEUE_H_INCLUDED
