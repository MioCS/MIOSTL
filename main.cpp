#include <iostream>
#include <vector>
#include <memory>
#include <new>
#include "mio_alloc.h"

using namespace std;

int main()
{
    //void *p = A<3>::allocate(3);

    void *p = second_alloc::allocate(3);
    cout << second_alloc::get_heapSize() << endl;

    //second_alloc::deallocate(p, 0);

    return 0;
}
