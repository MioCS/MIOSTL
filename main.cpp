#include <iostream>
#include <vector>
#include <stack>
#include <memory>
#include <new>
#include <algorithm>

#include "mio_vector.h"
#include "mio_list.h"
#include "mio_deque.h"
#include "mio_allocator.h"
#include "mio_queue.h"

class Base
{
public:
    virtual void f() noexcept;
};

void Base::f() noexcept
{

}

class Derive : public Base
{
public:
    void f() noexcept override;
};

void Derive::f() noexcept
{

}

using namespace std;

int main()
{
    mio::queue<int> d(1, 3);
    cout << d.top() << endl;

    return 0;
}
