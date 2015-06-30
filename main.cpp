#include <iostream>
#include <vector>
#include <memory>
#include <new>
#include <algorithm>
#include "mio_list.h"

using namespace std;

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

    l1.splice(l1.begin(), l2);

    l1.show();
    l2.show();

    return 0;
}
