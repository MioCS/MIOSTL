#include <iostream>
#include <vector>
#include <stack>
#include <memory>
#include <new>
#include <iterator>

#include "mio_heap.h"

using namespace std;

int main()
{
    std::vector<int> vec{8, 7, 6, 5, 4, 3, 10};

    mio::push_heap(vec.begin(), vec.end());

    for(const auto &i : vec)
    {
        cout << i << endl;
    }

    mio::pop_heap(vec.begin(), vec.end());

    for(const auto &i : vec)
    {
        cout << i << endl;
    }

    return 0;
}
