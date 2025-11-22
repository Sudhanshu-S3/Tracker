#include <iostream>
#include <string>
#include <vector>
#include <span>


/*
Span:-

A lightweight , non-owning view of a contiguous block of memory. It only points to it.

It solves the problem of passing arrays safely without:
    -copying the data
    -allocating memory
    -decaying to an unsafe pointer

void foo(std::span<int> data)

This can accepts , a C array , a vector, a std::array
All without copying anything.

*/

/*
This Program is about to get the Understanding of Undefined behavior.

    1. unsafe code that returns a string_view to temporary data (bad)

    2. safe, modern C++20 technique using span + string_view (good)

*/


//Returns a reference to a temporary 
std::string_view getTickerBad()
{
    std::string message = "Msg: AAPL Price: 150";

    return std::string_view(message).substr(5,4);
}

// We pass a 'span' No heap allocation happens here.

constexpr std::string_view getTickerGood(std::string_view message)
{
    return message.substr(5,4);
}

// work only for C++20 or above use to compile => g++ -std=c++23 main.cpp -o app


int main()
{
    char raw[]= "Msg: AAPL Price: 150";

    std::span<char> bufferSpan{raw};

    std::string_view view(bufferSpan.data(), bufferSpan.size());

    auto ticker = getTickerGood(view);

    std::cout<< "Ticker: " << ticker << "\n";



    // To check the cpp version.

    //long cpp_version = __cplusplus;

    //std::cout<< cpp_version << "\n";

    return 0;

}


/*

std::string_view is just two numbers: a pointer to the start of the string and the length.
It fits in two CPU registers. Passing it is essentially free.

*/