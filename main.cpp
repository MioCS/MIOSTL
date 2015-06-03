#include <iostream>
#include <vector>
#include <memory>
#include <new>
#include "mio_alloc.h"

using namespace std;

template <int x>
class A
{
public:
    static void *allocate(size_t n)
    {
        return std::malloc(n);
    }
};

typedef mio::__default_alloc_template<false, 0> second_alloc;

int main()
{
    //void *p = A<3>::allocate(3);

    void *p = second_alloc::allocate(3);
    cout << second_alloc::get_heapSize() << endl;

    for(int i = 0; i < 18; ++i)
    {
        second_alloc::allocate(3);
    }
    cout << second_alloc::get_heapSize() << endl;

    //second_alloc::deallocate(p, 0);

    return 0;
}
