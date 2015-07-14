#include <iostream>
#include <vector>
#include <memory>
#include <new>
#include <algorithm>
#include "mio_vector.h"
#include "mio_list.h"
#include "mio_deque.h"

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
    mio::deque<int> dq(63, 1);

    dq.push_front(3);
    cout << dq.front() << endl;

    return 0;
}
