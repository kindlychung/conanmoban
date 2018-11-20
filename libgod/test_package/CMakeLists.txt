
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(proj_name example)
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

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
include(cotire)

add_executable(${proj_name} ${proj_name}.cpp)
message(CONAN_LIBS ": ${CONAN_LIBS}")
target_link_libraries(${proj_name} ${CONAN_LIBS})

cotire(${proj_name})
