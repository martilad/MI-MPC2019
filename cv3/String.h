#ifndef STRING_H
#define STRING_H

#include <ostream>

class String
{
public:
    String(const char* = "");
    String(const String&);
    String(String&&) noexcept;
    String& operator=(const String&);
    String& operator=(String&&) noexcept;
    ~String();
    void swap(String&) noexcept;
    const char* data() const;
private:
    class Impl;
    Impl* pimpl_;
};

void swap(String&, String&) noexcept;

std::ostream& operator<<(std::ostream& os, const String& s);

#endif