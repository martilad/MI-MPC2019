#ifndef DEEP_PTR_H
#define DEEP_PTR_H

#include <utility>

template <typename T>
class deep_ptr
{
   T* ptr_;

public:
   deep_ptr() : ptr_(nullptr) { }
   deep_ptr(T* ptr) : ptr_(ptr) { }
   deep_ptr(deep_ptr&& other) : ptr_(other.ptr_) { other.ptr_ = nullptr; }

   deep_ptr(const deep_ptr& other) : ptr_(new T(*other)) {} // ??? TBD - DONE

   ~deep_ptr() { delete ptr_; }

   deep_ptr& operator=(deep_ptr other)
   {
      std::swap(ptr_, other.ptr_);
      return *this;
   }

   T* get() const { return ptr_; }
   T& operator*() const { return *ptr_; }
};

// ??? TBD - function make_deep - DONE
template<typename T, typename... Args>
deep_ptr<T> make_deep(Args&&... args) {
    return deep_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif
