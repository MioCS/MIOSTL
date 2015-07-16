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
private:
    int x;
public:
    void func(P &p)
    {
        x = p.x;
    }
};

int main()
{
    mio::deque<double> dq(10, 1);
    dq.push_front(2);
    dq.push_front(2);
    dq.clear();
    dq.push_back(1);
    dq.push_back(2);
    dq.erase(dq.begin());
    cout << dq.back() << endl;
    cout << dq.front() << endl;

    return 0;
}
