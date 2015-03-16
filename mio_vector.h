#ifndef MIO_VECTOR_H_INCLUDED
#define MIO_VECTOR_H_INCLUDED

namespace mio
{
    template <class T>
    class vector
    {
    typedef unsigned int size_t;
    public:
        vector(int c);
        vector(size_t c, T data);
        ~vector();

        T &operator[](size_t pos);

    private:
        void init();

        T *element;
        size_t capacity;
        size_t size;
    };
}

#endif // MIO_VECTOR_H_INCLUDED
