#include <string>
std::string hello_lib_cpp = R"###(
int sum(int a, int b) {
    return  a + b;
}
)###";

std::string hello_lib_h = R"###(
#pragma once
int sum(int a, int b);
)###";