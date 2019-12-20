#include <functional>
#include <iostream>
#include <string>
#include <tuple>

template <class U>
inline std::size_t hash_c(U const& v){
    return std::hash<U>()(v);
}

template <typename T, size_t N = std::tuple_size<T>::value - 1, size_t I = 0>
struct hash {
    std::size_t operator()(const T& t) {
        return hash_c(std::get<I>(t)) ^ hash<T, N, I + 1>{}(t); // process next element
    }
};

template <typename T, size_t N>
struct hash<T, N, N> {
    std::size_t operator()(const T& t) {
        return  hash_c(std::get<N>(t));
    }
};

int main()
{
   // CASE 1:
   auto t1 = std::make_tuple(1, 2.0, false);

   size_t h11 = std::hash<int>{}(1)
              ^ std::hash<double>{}(2.0)
              ^ std::hash<bool>{}(false);

   size_t h12 = std::hash< std::tuple_element_t<0, decltype(t1)> >{}(std::get<0>(t1))
              ^ std::hash< std::tuple_element_t<1, decltype(t1)> >{}(std::get<1>(t1))
              ^ std::hash< std::tuple_element_t<2, decltype(t1)> >{}(std::get<2>(t1));

   size_t h13 = hash< decltype(t1) >{}(t1);

   std::cout << h11 << std::endl << h12 << std::endl << h13 << std::endl << std::endl;

   // CASE 2:
   auto t2 = std::make_tuple<int, std::string>(-1, "hello");

   size_t h21 = std::hash<int>{}(-1)
              ^ std::hash<std::string>{}("hello");

   size_t h22 = std::hash< std::tuple_element_t<0, decltype(t2)> >{}(std::get<0>(t2))
              ^ std::hash< std::tuple_element_t<1, decltype(t2)> >{}(std::get<1>(t2));

   size_t h23 = hash< decltype(t2) >{}(t2);

   std::cout << h21 << std::endl << h22 << std::endl << h23 << std::endl << std::endl;
}
