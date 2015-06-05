#ifndef ITER_TEST_H_INCLUDED
#define ITER_TEST_H_INCLUDED

template <typename T>
class MyIter
{
public:
    T *_ptr;

    MyIter(T *ptr = nullptr):
        _ptr(ptr)
    {
    }

    T *operator->() const
    {
        return _ptr;
    }

    T &operator*() const
    {
        return *_ptr;
    }

    MyIter &operator++()
    {
        ++_ptr;

        return *this;
    }

    MyIter &operator++(int)
    {
        ++_ptr;

        return *this;
    }

    bool operator==(const MyIter &myIter) const
    {
        return _ptr == myIter._ptr;
    }

    bool operator!=(const MyIter &myIter) const
    {
        return _ptr != myIter._ptr;
    }
}

#endif // ITER_TEST_H_INCLUDED
