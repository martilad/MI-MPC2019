#include <iostream>
#include "String.h"

int main()
{
    String s1("hello");
    std::cout << s1 << std::endl;

    String s2(s1);
    std::cout << s2.data() << std::endl;

    String s3(std::move(s1));
    // s1 will not be used any more (expect of its destruction)
    std::cout << s3.data() << std::endl;

    String s4("world");
    swap(s3, s4);
    std::cout << s4.data() << " " << s3.data() << std::endl;

    String s_temp(std::move(s4));
    s4 = std::move(s3);
    s3 = std::move(s_temp);
    std::cout << s3.data() << " " << s4.data() << std::endl;

    s_temp = s4;
    s4 = s3;
    s3 = s_temp;
    std::cout << s4.data() << " " << s3.data() << std::endl;

    std::cout << sizeof(String) << std::endl;
}