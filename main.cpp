#include <iostream>
#include <vector>
#include <memory>
#include <new>
#include "mio_list.h"

using namespace std;

int main()
{
    mio::list<int> l;
    l.push_back(3);
    l.push_back(2);
    l.push_back(1);

    for(auto i = l.begin(); i != l.end(); ++i)
    {
        cout << *i << endl;
    }

    return 0;
}
