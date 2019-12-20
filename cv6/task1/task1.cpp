#include <cassert>
#include <iostream>
#include "deep_ptr.h"

struct X
{
   int i_;
   X(int i) : i_(i) { std::clog << "default ctor: address = " << this << ", value = " << i_ << std::endl; }
   X(const X& other) : i_(other.i_) { std::clog << "copy ctor: src addr = " << &other << ", dst addr = " << this << ", value = " << i_ << std::endl; }
   X(X&& other) : i_(other.i_) { std::clog << "move ctor: src addr = " << &other << ", dst addr = " << this << ", value = " << i_ << std::endl; }
   ~X() { std::clog << "dtor: address = " << this << ", value = " << i_ << std::endl; }
};

int main()
{
   { deep_ptr<X> x{ new X{1} }; }
   { auto x = make_deep<X>(1); }

   auto x = make_deep<X>(3);

   auto xc{x};
   assert(x.get() != xc.get());

   X* temp = x.get();
   auto xm{std::move(x)};
   assert(x.get() == nullptr);
   assert(xm.get() == temp);
}
