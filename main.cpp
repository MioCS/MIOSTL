#include <iostream>
#include <vector>
#include <memory>
#include <new>
#include <algorithm>
#include "mio_list.h"

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
    mio::list<int> l1;
    mio::list<int> l2;

    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    l1.push_back(3);
    l2.push_back(4);
    l2.push_back(4);
    l2.push_back(2);

    l1.show();
    l2.show();

    l1.swap(l2);

    l1.show();
    l2.show();

    P p, q;
    p.func(q);

    return 0;
}
