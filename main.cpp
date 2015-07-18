#include <iostream>
#include <vector>
#include <memory>
#include <new>
#include <algorithm>

#include "mio_vector.h"
#include "mio_list.h"
#include "mio_deque.h"
#include "mio_allocator.h"

using namespace std;

class P
{
public:
    void operator()(int x)
    {
        cout << x << endl;
    }
};

int main()
{
    mio::deque<double> dq(10, 1);
    dq.insert(dq.end() - 3, 2);
    for(auto i : dq)
    {
        cout << i << endl;
    }

    return 0;
}
