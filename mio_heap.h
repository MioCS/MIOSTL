#ifndef MIO_HEAP_H_INCLUDED
#define MIO_HEAP_H_INCLUDED

#include "mio_iterator.h"

namespace mio
{

// 以下为push_heap()实现
template <class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance insertIndex,
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

template <class RandomAccessIterator, class Distance, class T>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last,
                            Distance *, T*)
{
    __push_heap(first, (last - first - 1), Distance(0), T(*(last - 1)));
}

template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __push_heap_aux(first, last, distance_type(first), value_type(first));
}

template <class RandomAccessIterator, class Distance, class T>
inline void __adjust_heap(RandomAccessIterator first, Distance adjustIndex, Distance len, T value)
{
    Distance topIndex = adjustIndex;
    Distance secondChild = topIndex * 2 + 2;  // 待调整节点的右孩子

    // 将最后一个元素(出堆元素的前一个元素)之前的部分调整为大顶堆
    while(secondChild < len)
    {
        // 令secondChild指向待调整节点左右孩子中的较大者
        if(*(first + secondChild - 1) > *(first + secondChild))
        {
            --secondChild;
        }

        *(first + adjustIndex) = *(first + secondChild);
        adjustIndex = secondChild;
        secondChild = secondChild * 2 + 2;
    }

    if(secondChild == len)
    {
        // 没有右节点，只有左节点
        *(first + adjustIndex) = *(first + (secondChild - 1));
        adjustIndex = secondChild - 1;
    }

    __push_heap(first, adjustIndex, topIndex, value);
}

// 以下为pop_heap()实现
template <class RandomAccessIterator, class T, class Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last,
                       RandomAccessIterator result, T value, Distance*)
{
    *result = *first;  // 将尾值设为欲求结果
    __adjust_heap(first, Distance(0), Distance(last - first), value);
}

template <class RandomAccessIterator, class T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
{
    __pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}

template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __pop_heap_aux(first, last, value_type(first));
}

// 以下为sort_heap()实现
template <class RandomAccessIterator>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    while(last - first > 1)
    {
        pop_heap(first, last--);
    }
}

// 以下为make_heap()实现
template <class RandomAccessIterator, class T, class Distance>
inline void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*)
{
    if(last - first < 2)
    {
        // 长度为0或1无需重排
        return;
    }

    Distance len = last - first;
    Distance adjustIndex = (len - 2) / 2;

    while(true)
    {
        //将以adjustIndex为首的树调整为大顶堆
        __adjust_heap(first, adjustIndex, len, *(first + adjustIndex));

        if(adjustIndex-- == 0)
        {
            // 调整至根节点时结束
            break;
        }
    }
}

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __make_heap(first, last, value_type(first), distance_type(first));
}

}  // end of namespace std

#endif // MIO_HEAP_H_INCLUDED
