#include <iostream>
#include "Vector.h"

using namespace std;

int main()
{
    mio::vector<int> s(10, 2);
    cout << s[2];
    return 0;
}
