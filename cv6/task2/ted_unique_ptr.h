#ifndef TED_UNIQUE_PTR_H
#define TED_UNIQUE_PTR_H

#include <memory>
#include <utility>

template <typename T>
class ted_unique_ptr
{
   struct DeleterBase
   {
      // ??? TBD (function for application of deleter to a pointer) - DONE
      virtual void delPtr(T* ptr) = 0;
      virtual ~DeleterBase() = default;
   };

   template <typename U>
   struct DeleterDerived : DeleterBase
   {
      // ??? TBD (store a copy of deleter) - DONE
      U f_;
      DeleterDerived(const U& deleter): f_(deleter){} // ??? TBD (create a copy of deleter)  - DONE

      void delPtr(T* ptr){
            f_(ptr);
      }
      // ??? TBD (function for application of (a stored copy of deleter) to a pointer) - DONE
   };

   T* ptr_;
   DeleterBase* ptr_del_;

public:
   ted_unique_ptr() : ptr_(nullptr), ptr_del_(nullptr) { }

   template <typename U = std::default_delete<T>>
   ted_unique_ptr(T* ptr, const U& deleter = U{}) : ptr_(ptr)
   {
       ptr_del_ = new DeleterDerived(deleter); // ??? TBD (create a type-erased storage for a copy of deleter) - DONE
   }

   ted_unique_ptr(const ted_unique_ptr&) = delete;

   ted_unique_ptr(ted_unique_ptr&& other) : ptr_(other.ptr_), ptr_del_(other.ptr_del_)
   {
      other.ptr_ = nullptr;
      other.ptr_del_ = nullptr;
   }

   ted_unique_ptr& operator=(const ted_unique_ptr&) = delete;

   ted_unique_ptr& operator=(ted_unique_ptr&& other)
   {
      ted_unique_ptr temp{ std::move(other) };
      std::swap(ptr_, temp.ptr_);
      std::swap(ptr_del_, temp.ptr_del_);
      return *this;
   }

   ~ted_unique_ptr()
   {
        if (ptr_del_ != nullptr){
            ptr_del_->delPtr(ptr_);
        }
      // ??? TBD (apply deleter to ptr_ (if not null) and perform clean up)  - DONE
   }
};

#endif
