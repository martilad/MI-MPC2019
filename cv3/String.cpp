#include <cstring>
#include <iostream>
#include "String.h"


// NO SSO String implementation
/*class String::Impl
{
    char* data_;

public:
    Impl(const char* arg): data_(new char[strlen(arg) + 1]) {
        strcpy(data_, arg);
    }
    Impl(const Impl& other): Impl(other.data()) {

    }
    Impl(Impl&& other) noexcept : data_(other.data_) {
        other.data_ = nullptr;
    }
    const char* data() const {
        return data_;
    }
    ~Impl() {
        delete[] data_;
    }
};
*/

// BUF SSO String implementation
class String::Impl {
    char* data_;
    char buf_[8];
public:
    Impl(const char* ptr) {
        data_ = strlen(ptr) < 8 ? buf_ : new char[strlen(ptr) + 1];
        strcpy(data_, ptr);
    }
    Impl(const Impl& other): Impl(other.data()) {

    }
    ~Impl() {
        if (data_ != buf_) delete[] data_;
    }
    const char* data() const {
        return data_;
    }
};


// TODO: write definitions of functions from String.h

String::String(const char* arg): pimpl_(new Impl(arg)) {

}
String::String(const String& other): String(other.data()) {

}
String::String(String&& other) noexcept : pimpl_(new Impl(std::move(*other.pimpl_))) {

}

String& String::operator=(const String& other){ // copy assignment operator
    String temp(other); // invoke copy constructor
    this->swap(temp); // swap contents
    return *this;
}

String& String::operator=(String&& other) noexcept{ // copy assignment operator
    String temp(std::move(other));  // move ctor invoked
    this->swap(temp);  // swap contentss
    return *this;
}

const char* String::data() const {
    return pimpl_->data();
}

void String::swap(String& other) noexcept {
    std::swap(pimpl_, other.pimpl_);
}

String::~String() {
    delete pimpl_;
    this->pimpl_ = nullptr;
}

void swap(String& l, String& r) noexcept{
    l.swap(r);
}

std::ostream& operator<<(std::ostream& os, const String& s) {
    os << s.data();
    return os;
}