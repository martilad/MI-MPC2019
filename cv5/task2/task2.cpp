#include <iostream>
#include <boost/type_index.hpp>
#include "Tuple.h"

using boost::typeindex::type_id_with_cvr;

int main()
{
   using T1 = Tuple<int&, double, bool>;

   std::cout << type_id_with_cvr< TupleElement<0, T1>::type >().pretty_name() << std::endl;
   std::cout << type_id_with_cvr< TupleElement<1, T1>::type >().pretty_name() << std::endl;
   std::cout << type_id_with_cvr< TupleElement<2, T1>::type >().pretty_name() << std::endl;

   using T2 = Tuple<const long&, float>;
   std::cout << type_id_with_cvr< TupleElement<0, T2>::type >().pretty_name() << std::endl;
   std::cout << type_id_with_cvr< TupleElement<1, T2>::type >().pretty_name() << std::endl;
}
