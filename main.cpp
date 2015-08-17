#include <iostream>
#include <vector>
#include <stack>
#include <memory>
#include <new>
#include <iterator>

#include "mio_priority_queue.h"

using namespace std;

int main()
{
    vector<int> vec{1, 2, 3, 4, 5, 6, 7};

    //mio::make_heap(vec.begin(), vec.end());

    mio::priority_queue<int, vector<int>> pq(vec.begin(), vec.end());

    for(auto& i : pq.c)
    {
        cout << i << ends;
    }
    cout << endl;

    pq.pop();

    for(auto& i : pq.c)
    {
        cout << i << ends;
    }

    return 0;
}
