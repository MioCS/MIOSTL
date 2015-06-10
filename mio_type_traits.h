#ifndef MIO_TYPE_TRAITS_H_INCLUDED
#define MIO_TYPE_TRAITS_H_INCLUDED

namespace mio
{

struct __true_type
{
};

struct __false_type
{
};

template <class type>
struct __type_trait
{
    typedef __false_type has_trivial_default_constructor;
    typedef __false_type has_trivial_copy_constructor;
    typedef __false_type has_trivial_assignmetn;
};

}

#endif // MIO_TYPE_TRAITS_H_INCLUDED
