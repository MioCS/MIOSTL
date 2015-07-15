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
    mio::deque<int> dq(10, 1);
    dq.push_back(2);
    cout << dq.back() << endl;
    dq.pop_back();
    cout << dq.back() << endl;

    return 0;
}
