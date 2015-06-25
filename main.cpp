#include <iostream>
#include <vector>
#include <memory>
#include <new>
#include <algorithm>
#include "mio_list.h"

using namespace std;

int main()
{
    mio::list<int> l;

    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(3);
    l.push_back(4);
    l.push_back(4);
    l.push_back(2);

    l.show();

    auto iter = std::find(l.begin(), l.end(), 2);

    l.insert(iter, 99);

    l.show();

    l.unique();

    l.show();

    return 0;
}
