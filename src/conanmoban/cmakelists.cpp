#include <inja.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include "add_data.h"

std::string cmake_commons{R"reuw-1437fhdu2n(
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(proj_name {{ proj_name }})
project(${proj_name} CXX)
set(conan_cmake_file "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
if(EXISTS ${conan_cmake_file})
    include(${conan_cmake_file})
    conan_basic_setup()
endif()
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
endif ()
)reuw-1437fhdu2n"};

std::string include_cotire = R"reuw-1437fhdu2n(
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
include(cotire)
)reuw-1437fhdu2n";

std::string cmake_proj_for_binary = R"reuw-1437fhdu2n(
add_executable(${proj_name} ${proj_name}/${proj_name}.cpp)
message(CONAN_LIBS ": ${CONAN_LIBS}")
target_link_libraries(${proj_name} ${CONAN_LIBS})
)reuw-1437fhdu2n";

std::string exec_cotire = R"reuw-1437fhdu2n(
cotire(${proj_name})
)reuw-1437fhdu2n";

std::string cmakelists_binary_render(std::string proj_name) {
    nlohmann::json data;
    AddData(proj_name);
    return inja::render(
        cmake_commons + include_cotire + cmake_proj_for_binary + exec_cotire,
        data);
}