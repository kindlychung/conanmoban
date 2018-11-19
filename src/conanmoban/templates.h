#pragma once
#include <string>
#include "proj_type.h"

std::string conanfile_py_header_only(
    std::string proj_name, std::string author_name, std::string author_email,
    std::string github_username, std::string topic, std::string description);
std::string conanfile_py_binary_render(
    std::string proj_name, std::string author_name, std::string author_email,
    std::string github_username, std::string topic, std::string description);

std::string conanfile_py_test_package(std::string proj_name);
std::string conanfile_py_lib(std::string proj_name, std::string author_name,
                             std::string author_email,
                             std::string github_username, std::string topic,
                             std::string description);

std::string test_package_cpp_render(std::string proj_name);

extern std::string hello_world_cpp;
extern std::string hello_header_only;

extern std::string hello_lib_cpp;
extern std::string hello_lib_h;

extern std::string cotire_cmake;
extern std::string git_ignore;
extern std::string git_attr;

extern std::string hello_world_cpp;

std::string cmakelists_render(std::string proj_name, ProjType ptype,
                              bool nest_proj_in_src = true);