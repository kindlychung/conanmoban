#include <inja.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include "add_data.h"

std::string test_package_cpp = R"###(
#include <{{ proj_name }}/{{ proj_name }}.h>
#include <cassert>
#include <iostream>
int main() {
    int x = 1;
    int y = 2;
    std::cout << sum(x, y) << std::endl;
}
)###";

std::string test_package_cpp_render(std::string proj_name) {
    nlohmann::json data;
    AddData(proj_name);
    return inja::render(test_package_cpp, data);
}