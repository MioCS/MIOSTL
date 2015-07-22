#ifndef MIO_HEAP_H_INCLUDED
#define MIO_HEAP_H_INCLUDED

#include "mio_iterator.h"

namespace mio
{

template <class RandomAcessIterator, class Distance, class T>
void __push_heap(RandomAcessIterator first, Distance insertIndex,
                 Distance topIndex, T value)
{
    Distance parent = (insertIndex - 1) / 2;  // 找出父节点

    while(insertIndex > topIndex && *(first + parent) < value)
    {
        // 尚未到达顶端，且插入元素值大于父节点，继续向上调整大顶堆
        *(first + insertIndex) = *(first + parent);
        insertIndex = parent;
        parent = (insertIndex - 1) / 2;
    }

    *(first + insertIndex) = value;
}

template <class RandomAcessIterator, class Distance, class T>
inline void __push_heap_aux(RandomAcessIterator first, RandomAcessIterator last,
                            Distance *, T*)
{
    __push_heap(first, (last - first - 1), Distance(0), T(*(last - 1)));
}

template <class RandomAcessIterator>
inline void push_heap(RandomAcessIterator first, RandomAcessIterator last)
{
    __push_heap_aux(first, last, distance_type(first), value_type(first));
}

}  // end of namespace std



#endif // MIO_HEAP_H_INCLUDED
