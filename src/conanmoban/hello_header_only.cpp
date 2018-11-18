#include <string>
std::string hello_header_only = R"###(
#pragma once
template <typename T>
T sum(T a, T b) {
    return a + b;
}
)###";
