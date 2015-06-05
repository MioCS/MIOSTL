#include <iostream>
#include <vector>
#include <memory>
#include <new>

//
using namespace std;

template <typename T>
void func()
{
    cout << "p" << endl;
}

template <typename T>
void func<T*>()
{
    cout << "r" << endl;
}

int main()
{

    return 0;
}
