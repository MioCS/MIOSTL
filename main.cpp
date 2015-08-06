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
    vector<int> vec{1, 2, 3, 4, 5, 6};

    mio::make_heap(vec.begin(), vec.end());

    for(const auto &i : vec)
    {
        cout << i << ends;
    }

    return 0;
}
