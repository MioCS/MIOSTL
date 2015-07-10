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
    mio::deque<double> dq(10000, 1.0);

    cout << dq.front() << endl;

    int x = 3;
    int *y = &x;

    cout << y << endl;
    cout << (y + 1) << endl;

    return 0;
}
