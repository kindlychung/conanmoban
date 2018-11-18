#include <string>
std::string hello_header_only = R"###(
template <typename T>
T sum(T a, T b) {
    return a + b;
}
)###";
