#ifndef MIO_ALLOC_H_INCLUDED
#define MIO_ALLOC_H_INCLUDED

#include <cstdlib>
#include <new>

namespace mio
{

/**
 *  第一级配置器
 */
template <int inst>
class __malloc_alloc_template
{
private:
    // 以下函数用来处理内存不足情况
    static void *oom_malloc(size_t);

    static void *oom_realloc(void *, size_t);

    static void (* __malloc_alloc_oom_handler)();

public:
    static void *allocate(size_t n)
    {
        void *result = std::malloc(n);

        if(result == nullptr)
        {
            result = oom_malloc(n);
        }

        return result;
    }

    static void deallocate(void *p, size_t /* n */)
    {
        std::free(p);
    }

    static void *reallocate(void *p, size_t /* old_sz */, size_t new_sz)
    {
        void *result = std::realloc(p, new_sz);

        if(result == nullptr)
        {
            result = oom_realloc(p, new_sz);
        }

        return result;
    }

    static void (*set_malloc_handle(void (*f)()))()
    {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;

        return old;
    }
};

template <int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = nullptr;

template <int inst>
void *__malloc_alloc_template<inst>::oom_malloc(size_t n)
{
    void (*my_malloc_handler)() = nullptr;
    void *result = nullptr;

    while(true)
    {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if(my_malloc_handler == nullptr)
        {
            std::bad_alloc error;
            throw error;
        }
        (*my_malloc_handler)();

        result = std::malloc(n);
        if(result)
        {
            return result;
        }
    }
}

template <int inst>
void *__malloc_alloc_template<inst>::oom_realloc(void *p, size_t n)
{
    void (*my_malloc_handler)() = nullptr;
    void *result = nullptr;

    while(true)
    {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if(my_malloc_handler == nullptr)
        {
            std::bad_alloc error;
            throw error;
        }
        (*my_malloc_handler)();

        result = std::realloc(p, n);
        if(result)
        {
            return result;
        }
    }
}

typedef __malloc_alloc_template<0> malloc_alloc;

/**
 *  次级配置器
 */
template <bool threads, int inst>
class __default_alloc_template
{
private:
    enum {__ALIGN = 8};  // 小型区块的上调边界
    enum {__MAX_BYTES = 128};  // 小型区块的上限
    enum {__NFREELISTS = __MAX_BYTES / __ALIGN}; // free-lists 个数

    union obj
    {
        union obj *freeListLink;
        char client_data[1];
    };

    // 将 bytes 上调为 8 的倍数
    static size_t round_up(size_t bytes)
    {
        return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
    }

    static obj * volatile freeList[__NFREELISTS];

    // 根据区块大小，决定使用第n号free-list
    static size_t freelist_index(size_t bytes)
    {
        return ((bytes + __ALIGN - 1) / __ALIGN - 1);
    }

    // 请求分配多个大小为n的内存块，多余块会加入到对应freelist项中
    static void *refill(size_t n);

    // 从内存池中调度空间给freelist
    static char *chunk_alloc(size_t size, int &nobjs);

    static char *startFree;
    static char *endFree;
    static size_t heapSize;

public:
    static void *allocate(size_t n)
    {
        // 大于128调用第一级配置器
        if(n > static_cast<size_t>(__MAX_BYTES))
        {
            return (malloc_alloc::allocate(n));
        }

        obj *result;
        obj * volatile *myFreeList;

        // 在16个free_list中寻找合适的
        myFreeList = freeList + freelist_index(n);
        result = *myFreeList;

        if(result == nullptr)
        {
            // 没有找到可用的freelist,重新填充freelist
            void *r = refill(round_up(n));
            return r;
        }

        *myFreeList = result->freeListLink;

        return result;
    }

    static void deallocate(void *p, size_t n)
    {
        if(n > static_cast<size_t>(__MAX_BYTES))
        {
            malloc_alloc::deallocate(p, n);

            return;
        }

        obj *q = static_cast<obj *>(p);
        obj * volatile *myFreeList;

        // 找到对应大小的 freelist
        myFreeList = freeList + freelist_index(n);

        // 调整reelist 回收区块
        q->freeListLink = *myFreeList;
        *myFreeList = q;
    }

    static void *realloc(void *p, size_t old_sz, size_t new_sz);

    static size_t get_heapSize()
    {
        return heapSize;
    }
};

// member of __default_alloc_template
template <bool threads, int inst>
char *__default_alloc_template<threads, inst>::startFree = nullptr;

template <bool threads, int inst>
char *__default_alloc_template<threads, inst>::endFree = nullptr;

template <bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heapSize = 0;

template <bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj * volatile
__default_alloc_template<threads, inst>::freeList[__NFREELISTS] =
{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
 nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

template <bool threads, int inst>
void *__default_alloc_template<threads, inst>::refill(size_t n)
{
    // 默认获得20个块，内存不足时会减少
    int nobjs = 20;

    // 请求分配20个新块, nobjs为引用类型可能在分配不足时被更改
    char * chunk = chunk_alloc(n, nobjs);

    // 只获得一个区块，直接分配给调用者，freelist不更新
    if(nobjs == 1)
    {
        return chunk;
    }

    obj *result = reinterpret_cast<obj *>(chunk);  //

    // 获得多个区块，准备更新freelist
    obj * volatile *myFreeList;
    obj *currentObj, *nextObj;

    myFreeList = freeList + freelist_index(n);
    *myFreeList = nextObj = reinterpret_cast<obj *>(chunk + n);  //

    // 在获得内存块中构造freelist
    for(int i = 1; i < nobjs; ++i)
    {
        currentObj = nextObj;
        /// 为了能以currentObj为基准正确移动n个字节，需要将其强制转换为char*
        /// 可替换为nextObj = currentObj + 1;
        nextObj = reinterpret_cast<obj *>(reinterpret_cast<char *>(currentObj) + n);
        currentObj->freeListLink = nextObj;
    }
    currentObj->freeListLink = nullptr;  // 最后一块

    return result;
}

template <bool threads, int inst>
char *__default_alloc_template<threads, inst>::chunk_alloc(size_t size, int &nobjs)
{
    char *result;
    size_t totalBytes = size * nobjs;
    size_t bytesLeft = endFree - startFree;

    if(bytesLeft > totalBytes)
    {
        // 内存池空间满足需求
        result = startFree;
        startFree += bytesLeft;

        return result;
    }
    else if(bytesLeft >= size)
    {
        // 内存池剩余空间不能完全满足需求，但能提供 >= 1 个块
        nobjs = bytesLeft / size;
        totalBytes = size * nobjs;
        result = startFree;
        startFree += totalBytes;

        return result;
    }

    // else 内存池剩余空间不足
    // 回收剩余零碎空间
    if(bytesLeft > 0)
    {
        // 找到适当的位置
        obj * volatile *myFreeList = freeList + freelist_index(bytesLeft);
        (reinterpret_cast<obj *>(startFree))->freeListLink = *myFreeList;  //
        *myFreeList = reinterpret_cast<obj *>(startFree);  //
    }

    // 配置heap空间，补充内存池(两倍空间 + 随次数增加的附加量)
    size_t bytesToGet = totalBytes * 2 + round_up(heapSize >> 4);

    startFree = static_cast<char *>(malloc(bytesToGet));

    if(startFree == nullptr)
    {
        // heap空间不足，malloc失败，尝试在freelist中寻找足够大的块
        obj * volatile *myFreeList, *p;

        for(size_t i = size; i <= __MAX_BYTES; i += __ALIGN)
        {
            myFreeList = freeList + freelist_index(i);
            p = *myFreeList;

            if(p != nullptr)
            {
                // 存在尚未使用的块
                *myFreeList = p->freeListLink;
                startFree = reinterpret_cast<char *>(p);
                endFree = startFree + i;

                // 递归调用，修正nobjs，并处理可能存在的零碎多余空间
                return chunk_alloc(size, nobjs);
            }
        }

         // 没有任何可用空间，调用第一级配置器，尝试使用oom_handle
        endFree = nullptr;
        startFree = static_cast<char *>(malloc_alloc::allocate(bytesToGet));
    }

    heapSize += bytesToGet;
    endFree = startFree + bytesToGet;

    // 递归调用，修正nobjs
    return chunk_alloc(size, nobjs);
}

typedef __default_alloc_template<false, 0> default_alloc;

/**
 *  配置器包装接口
 */
template<class T, class Alloc>
class simple_alloc
{
public:
    static T *allocate(size_t n)
    {
        return n == 0 ? nullptr : static_cast<T *>(Alloc::allocate(n * sizeof(T)));
    }

    static T *allocate()
    {
        return static_cast<T *>(Alloc::allocate(sizeof(T)));
    }

    static void deallocate(T *p, size_t n)
    {
        if(n != 0)
        {
            Alloc::deallocate(p, n * sizeof(T));
        }
    }

    static void deallocate(T *p)
    {
        Alloc::deallocate(p, sizeof(T));
    }
};

}  //
#endif // MIO_ALLOC_H_INCLUDED
