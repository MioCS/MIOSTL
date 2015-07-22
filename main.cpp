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
    std::vector<int> vec{3, 2, 4};

    mio::push_heap(vec.begin(), vec.end());

    for(const auto &i : vec)
    {
        cout << i << endl;
    }

    return 0;
}
