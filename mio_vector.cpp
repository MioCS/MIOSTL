#include "mio_vector.h"

namespace mio
{
    template<class T>
    vector<T>::vector(int c):
        capacity(c), size(0)
    {
        init();
    }

    template<class T>
    vector<T>::vector(size_t c, T data):
        capacity(c), size(c)
    {
        init();
        for(size_t i = 0; i < capacity; ++i)
        {
            element[i] = data;
        }
    }

    template<class T>
    vector<T>::~vector()
    {
        delete [] element;
    }

    template<class T>
    T &vector<T>::operator[](size_t pos)
    {
        return element[pos];
    }


    /**
     * private function member
     */

    template<class T>
    void vector<T>::init()
    {
        element = new T[size];
    }
}
