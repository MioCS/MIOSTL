#ifndef MIO_ALLOC_H_INCLUDED
#define MIO_ALLOC_H_INCLUDED

#include <cstdlib>
#include <new>

namespace mio
{

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

        if(result == NULL)
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

        if(result == NULL)
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
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = NULL;

template <int inst>
void *__malloc_alloc_template<inst>::oom_malloc(size_t n)
{
    void (*my_malloc_handler)() = NULL;
    void *result = NULL;

    while(true)
    {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if(my_malloc_handler == NULL)
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
    void (*my_malloc_handler)() = NULL;
    void *result = NULL;

    while(true)
    {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if(my_malloc_handler == NULL)
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

}  //
#endif // MIO_ALLOC_H_INCLUDED
