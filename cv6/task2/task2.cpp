#include <cassert>
#include <cstddef>
#include <iostream>
#include <list>
#include "ted_unique_ptr.h"

struct X
{
   int i_;
   X(int i) : i_(i) { std::clog << "default ctor: address = " << this << ", value = " << i_ << std::endl; }
   ~X() { std::clog << "dtor: address = " << this << ", value = " << i_ << std::endl; }
};

int main()
{
   {
      ted_unique_ptr<X> p{ new X{-1} };
   }

   auto deleter1 = [](auto* ptr)
   {
      std::clog << "Deleting " << ptr << " by deleter 1" << std::endl;
      delete ptr;
   };

   auto deleter2 = [](auto* ptr)
   {
      std::clog << "Deleting " << ptr << " by deleter 2" << std::endl;
      delete ptr;
   };

   size_t deleter3_counter = 0;
   auto deleter3 = [&deleter3_counter](auto* ptr)
   {
      std::clog << "Deleting " << ptr << " by deleter 3 : counter = " << ++deleter3_counter <<  std::endl;
      delete ptr;
   };

   {
      std::list<ted_unique_ptr<X>> l;
      l.emplace_back(new X{1}, deleter1);
      l.emplace_back(new X{2}, deleter2);
      l.emplace_back(new X{3}, deleter3);
      l.emplace_back(new X{4}, deleter3);
   }
   assert(deleter3_counter == 2);
}
