#include <iostream>
#include <vector>
#include <memory>
#include <new>
#include "mio_vector.h"

using namespace std;

int main()
{
    mio::vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    v.insert(v.begin() + 2, 3, 10);
    v.erase(v.begin(), v.begin() + 2);

    for(auto i : v)
    {
        cout << i << endl;
    }

    return 0;
}
