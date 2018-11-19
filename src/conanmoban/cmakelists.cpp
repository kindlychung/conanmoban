#include <inja.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include "add_data.h"
#include "proj_type.h"

std::string cmake_commons{R"reuw-1437fhdu2n(
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(proj_name {{ proj_name }})
set(CMAKE_CXX_FLAGS "-g") 
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

std::string cmake_proj_for_lib = R"reuw-1437fhdu2n(
add_library(${proj_name} ${proj_name}/${proj_name}.cpp)
message(CONAN_LIBS ": ${CONAN_LIBS}")
target_link_libraries(${proj_name} ${CONAN_LIBS})
)reuw-1437fhdu2n";

std::string cmake_proj_for_binary = R"reuw-1437fhdu2n(
add_executable(${proj_name} ${proj_name}/${proj_name}.cpp)
message(CONAN_LIBS ": ${CONAN_LIBS}")
target_link_libraries(${proj_name} ${CONAN_LIBS})
)reuw-1437fhdu2n";

std::string cmake_proj = R"reuw-1437fhdu2n(
add_{{ proj_type }}(${proj_name} {{ namespace_dir }}${proj_name}.cpp)
message(CONAN_LIBS ": ${CONAN_LIBS}")
target_link_libraries(${proj_name} ${CONAN_LIBS})
)reuw-1437fhdu2n";

std::string cmake_proj_for_binary_non_nested = R"reuw-1437fhdu2n(
add_executable(${proj_name} ${proj_name}.cpp)
message(CONAN_LIBS ": ${CONAN_LIBS}")
target_link_libraries(${proj_name} ${CONAN_LIBS})
)reuw-1437fhdu2n";

std::string exec_cotire = R"reuw-1437fhdu2n(
cotire(${proj_name})
)reuw-1437fhdu2n";

std::string cmakelists_render(std::string proj_name, ProjType ptype,
                              bool nest_proj_in_src) {
    nlohmann::json data;
    AddData(proj_name);
    data["proj_type"] = ~ptype;
    data["namespace_dir"] = nest_proj_in_src ? (proj_name + "/") : "";
    return inja::render(
        cmake_commons + include_cotire + cmake_proj + exec_cotire, data);
}
