#ifndef MIO_VECTOR_H_INCLUDED
#define MIO_VECTOR_H_INCLUDED

#include "mio_alloc.h"
#include <bits/stl_uninitialized.h>
#include <bits/stl_construct.h>

namespace mio
{

typedef default_alloc alloc;

template <class T, class Alloc = alloc>
class vector
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef value_type& reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

protected:
    typedef simple_alloc<value_type, Alloc> data_allocator;  // �ռ�������
    iterator start;         // ��ʾʹ�ÿռ��ͷ
    iterator finish;        // ��ʾʹ�ÿռ��β
    iterator endOfStorage;  // ��ʾ���ÿռ��β

    void insert_aux(iterator position, const T& x);

    void deallocate()
    {
        if(start)
        {
            data_allocator::deallocate(start, endOfStorage - start);
        }
    }

    void fill_initialize(size_type n, const T& value)
    {
        start = allocate_and_fill(n, value);
        finish = start + n;
        endOfStorage = finish;
    }

    iterator allocate_and_fill(size_type n, const T& x)
    {
        iterator result = data_allocator::allocate(n);
        std::uninitialized_fill_n(result, n, x);

        return result;
    }

public:
    iterator begin()
    {
        return start;
    }

    iterator end()
    {
        return finish;
    }

    size_type size() const
    {
        return static_cast<size_type>(finish - start);
    }

    size_type capacity() const
    {
        return static_cast<size_type>(endOfStorage - start);
    }

    bool empty() const
    {
        return start == finish;
    }

    reference operator[](size_type n)
    {
        return *(start + n);
    }

    // constructors
    vector():
        start(nullptr), finish(nullptr), endOfStorage(nullptr)
    {
    }

    vector(size_type n, const T &value)
    {
        fill_initialize(n, value);
    }

    vector(int n, const T &value)
    {
        fill_initialize(n, value);
    }

    vector(long n, const T &value)
    {
        fill_initialize(n, value);
    }

    explicit vector(size_type n)
    {
        fill_initialize(n, value_type());
    }

    ~vector()
    {
        //TODO:
    }

    reference front()
    {
        return *start;
    }

    reference back()
    {
        return *finish;
    }

    void push_back(const T &x)
    {
        if(finish != endOfStorage)
        {
            std::_Construct(finish, x);
            ++finish;
        }
        else
        {
            insert_aux(finish, x);
        }
    }

    void pop_back()
    {
        --finish;
        std::_Destroy(finish);
    }

    iterator erase(iterator first, iterator last)
    {
        iterator i = std::copy(last, finish, first);
        std::_Destroy(i, finish);
        finish = i;

        return first;
    }

    iterator erase(iterator position)
    {
        if(position + 1 != finish)
        {
            copy(position + 1, finish, position);
            --finish;
            std::_Destroy(finish);

            return position;
        }
    }

    void insert(iterator position, size_type n, const T &x);

    void resize(size_type newSize, const T &x)
    {
        if(newSize < size())
        {
            erase(begin() + newSize, end());
        }
        else
        {
            insert(end(), newSize - size(), x);
        }
    }

    void resize(size_type newSize)
    {
        resize(newSize, value_type());
    }

    void clear()
    {
        erase(begin(), end());
    }
};

template <class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& x)
{
    if(finish != endOfStorage)
    {
        // �б��ÿռ�

        // ����ƶ����ݣ�����x
        std::_Construct(finish, *(finish - 1));
        ++finish;
        // copy [position, finish - 2) to [?, finish - 1)
        std::copy_backward(position, finish - 2, finish - 1);
        *position = x;
    }
    else
    {
        // �ޱ��ÿռ�
        // ����ԭ��2���Ŀռ�
        const size_type oldSize = size();
        const size_type len = oldSize ? 2 * oldSize : 1;

        iterator newStart = data_allocator::allocate(len);
        iterator newFinish = newStart;

        try
        {
            // ���������ǰ�Ĳ��ֵ��¿ռ�
            newFinish = std::uninitialized_copy(start, position, newStart);
            std::_Construct(newFinish, x);
            ++newFinish;
            // ����������Ĳ��ֵ��¿ռ�
            newFinish = std::uninitialized_copy(position, finish, newFinish);
        }
        catch(...)
        {
            // commit or rollback semantics
            std::_Destroy(newStart, newFinish);
            data_allocator::deallocate(newStart, len);
            throw;
        }

        // �������ͷ�ԭvector
        std::_Destroy(begin(), end());
        deallocate();

        // ������������ָ����vector
        start = newStart;
        finish = newFinish;
        endOfStorage = newStart + len;
    }
}

template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T &x)
{
    if(n == 0)
    {
        return;
    }

    // n������0�Ž������²���
    if((endOfStorage - finish) >= n)
    {
        T xCopy = x;
        // ��������֮������Ԫ�صĸ���
        const size_type elemsAfter = finish - position;
        iterator oldFinish = finish;

        if(elemsAfter > n)
        {
            // �����֮�������Ԫ�ظ�����������Ԫ�ظ���
            // ��n��Ԫ����Ҫ����
            std::uninitialized_copy(finish - n, finish, finish);
            finish += n;
            // ��n��Ԫ��ֱ�ӿ���
            std::copy_backward(position, oldFinish - n, oldFinish);
            std::fill(position, position + n, xCopy);
        }
        else
        {
            // ����oldFinish��n�Ĳ���
            std::uninitialized_fill_n(finish, n - elemsAfter, xCopy);
            finish += n - elemsAfter;
            // �ƶ�ԭ�в���
            std::uninitialized_copy(position, oldFinish, finish);
            finish += elemsAfter;
            std::fill(position, oldFinish, xCopy);
        }
    }
    else
    {
        // ���ÿռ䲻��
        const size_type oldSize = size();
        // �����³��ȣ��ɳ��ȵ�������ɳ���+����Ԫ�ظ���
        const size_type len = oldSize + std::max(oldSize, n);

        // �����¿ռ�
        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        try
        {
            // ����vector�в����֮ǰ��Ԫ�ظ��Ƶ��¿ռ�
            new_finish = std::uninitialized_copy(start, position, new_start);
            // ����Ԫ�������¿ռ�
            std::uninitialized_fill_n(new_finish, n, x);
            new_finish += n;
            // ����vector�в����֮���Ԫ�ظ��Ƶ��¿ռ�
            new_finish = std::uninitialized_copy(position, finish, new_finish);
        }
        catch(...)
        {
            std::_Destroy(new_start, new_finish);
            data_allocator::deallocate(new_start, len);
            throw;
        }

        // �¿ռ�������ɣ�����ɿռ�
        std::_Destroy(start, finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        endOfStorage = new_start + len;
    }
}
}
#endif // MIO_VECTOR_H_INCLUDED
