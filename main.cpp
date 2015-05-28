#include "mio_allocator.h"
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

int main()
{
    int nums[5] = {1, 2, 3, 4, 5};

    vector<int, mio::allocator<int>> numV(nums, nums + 100);

    for(auto i : numV)
    {
        cout << i << ends;
    }

    cout << int(3);

    return 0;
}
