#ifndef MIO_ITERATOR_H_INCLUDED
#define MIO_ITERATOR_H_INCLUDED

#include <iterator>

namespace mio
{

template <class Iterator>
inline typename std::iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
  typedef typename std::iterator_traits<Iterator>::iterator_category category;
  return category();
}

template <class Iterator>
inline typename std::iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
  return static_cast<typename std::iterator_traits<Iterator>::difference_type*>(0);
}

template <class Iterator>
inline typename std::iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
  return static_cast<typename std::iterator_traits<Iterator>::value_type*>(0);
}

}  // end of namespace mio

#endif // MIO_ITERATOR_H_INCLUDED
