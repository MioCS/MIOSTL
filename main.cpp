#include <iostream>
#include <vector>
#include <memory>

using namespace std;

int main()
{
    enum mm{x, y};

    mm d;
    d = x;

    cout << d;
    /*
    int nums[5] = {1, 2, 3, 4, 5};

    vector<int> numV(nums, nums + 100);

    for(auto i : numV)
    {
        cout << i << ends;
    }

    cout << int(3);
    */

    return 0;
}
