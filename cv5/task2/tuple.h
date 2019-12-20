#ifndef TUPLE_H
#define TUPLE_H

#include <cstddef>
#include <iostream>

template <typename T, typename... Ts>
class Tuple
{
   using value_type = T;
   using nested_type = Tuple<Ts...>;
   value_type t_;
   nested_type ts_;

   template <size_t, typename> friend struct TupleElement;

public:
   Tuple(const T& t, const Ts&... ts) : t_(t), ts_(ts...) { }
};

template <typename T>
class Tuple<T>
{
   using value_type = T;
   T t_;

   template <size_t, typename> friend struct TupleElement;

public:
   Tuple(const T& t) : t_(t) { }
};

// help functions
template <int N, typename... U>
struct get_type;

template <int N, typename T, typename... U>
struct get_type<N, Tuple<T, U...>> {
    using type = typename get_type<N - 1, Tuple<U...>>::type;
};

template <typename T, typename... U>
struct get_type<0, Tuple<T, U...>> {
    using type = T;
};
// TupleElement
template <size_t I, typename T>
struct TupleElement {
   using type = typename get_type<I, T>::type;
};

#endif
