#include <docopt.h>
#include <fmt/format.h>
#include <inja.hpp>
#include <iomanip>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include "default_opts.h"
#include "templates.h"

#include <filesystem>
namespace fs = std::filesystem;
using json = nlohmann::json;

#define VERSION "1.0.3"

static const char USAGE[] =
    R"####(Conan project templates. (c) 2018 Kaiyin Zhong

Usage:
   conanmoban bin <proj_name> [--author_name <author_name>] [--author_email <author_email>] [--github_username <github_username>] [--topic <topic>] [--description <description>]
   conanmoban lib <proj_name> [--author_name <author_name>] [--author_email <author_email>] [--github_username <github_username>] [--topic <topic>] [--description <description>] [--header_only]
   conanmoban (-h | --help)
   conanmoban --version

Options:
  -h --help                               Show this screen.
  --version                               Show version.
  --author_name <author_name>             Name of the author. This option and the ones below can also be given in a json file ($HOME/.conanmoban.json), for the format of {"author_name": "value", ...}. The value given on the commandline will take precedence. 
  --author_email <author_email>           Email of the author.
  --github_username <github_username>     Github handle.
  --topic <topic>                         Topic of the project
  --description <description>             Description of the project.
)####";

int main(int argc, char const** argv) {
    json user_config = default_opts();
    user_config.is_array();
    std::map<std::string, docopt::value> args = docopt::docopt(
        USAGE, {argv + 1, argv + argc},
        true,                                    // show help if requested
        fmt::format("conanmoban {}", VERSION));  // version string
    // std::cout << conanfile_py_content << std::endl;
    docopt::value temp_arg;

#define GetOpt(key)                                                \
    temp_arg = args["--" #key];                                    \
    std::string opt_##key =                                        \
        temp_arg ? temp_arg.asString()                             \
                 : (user_config.find("" #key) != user_config.end() \
                        ? user_config["" #key].get<std::string>()  \
                        : "change_your_" #key);

    auto proj_name = args["<proj_name>"].asString();
    GetOpt(author_name);
    GetOpt(author_email);
    GetOpt(github_username);
    GetOpt(topic);
    GetOpt(description);

    // setup directories
    auto proj_path = fs::path(proj_name);
    auto src_dir = proj_path / "src";
    auto build_dir = proj_path / "build";
    auto namespace_dir = src_dir / proj_name;
#define CreateDir(d)                                                     \
    if (!fs::create_directory((d))) {                                    \
        std::cerr << "failed to create directory: " << (d) << std::endl; \
        return 1;                                                        \
    }
    CreateDir(proj_path);
    CreateDir(src_dir);
    CreateDir(build_dir);
    CreateDir(namespace_dir);

    auto cotire_path = src_dir / "cotire.cmake";
    auto cmakelists_path = src_dir / "CMakeLists.txt";
    auto git_attr_path = proj_path / ".gitattributes";
    auto git_ignore_path = proj_path / ".gitignore";
    auto conanfile_py_path = proj_path / "conanfile.py";
    auto readme_path = proj_path / "readme.md";
    std::ofstream git_attr_ofs{git_attr_path};
    git_attr_ofs << git_attr;
    std::ofstream git_ignore_ofs{git_ignore_path};
    git_ignore_ofs << git_ignore;
    std::ofstream cotire_ofs{cotire_path};
    cotire_ofs << cotire_cmake;
    std::ofstream readme_ofs{readme_path};
    readme_ofs << "# " << proj_name;

    if (args["bin"].asBool()) {
        {  // write conanfile.py
            std::ofstream conanfile_py_ofs{conanfile_py_path};
            conanfile_py_ofs << conanfile_py_binary_render(
                proj_name, opt_author_name, opt_author_email,
                opt_github_username, opt_topic, opt_description);
        }
        {  // write src/CmakeLists.txt
            std::ofstream cmakelists_ofs{cmakelists_path};
            cmakelists_ofs << cmakelists_render(proj_name,
                                                ProjType::executable);
        }
        {  // write src/proj_name/proj_name.cpp
            auto hello_world_cpp_path =
                namespace_dir / fmt::format("{}.cpp", proj_name);
            std::ofstream hello_world_cpp_ofs{hello_world_cpp_path};
            hello_world_cpp_ofs << hello_world_cpp;
        }
    } else if (args["lib"].asBool()) {
        // for a library project there is a test_package dir
        auto test_package_dir = proj_path / "test_package";
        CreateDir(test_package_dir);
        {  // write test_package/conanfile.py
            auto test_conanfile_path = test_package_dir / "conanfile.py";
            std::ofstream test_conanfile_ofs{test_conanfile_path};
            test_conanfile_ofs << conanfile_py_test_package(proj_name);
        }
        {  // write test_package/CmakeLists.txt
            auto test_cmake_path = test_package_dir / "CMakeLists.txt";
            std::ofstream test_cmake_ofs{test_cmake_path};
            test_cmake_ofs << cmakelists_render("example", ProjType::executable,
                                                false);
        }
        {  // write test_package/example.cpp
            auto test_cpp_path = test_package_dir / "example.cpp";
            std::ofstream test_cpp_ofs{test_cpp_path};
            test_cpp_ofs << test_package_cpp_render(proj_name);
        }
        {  // write test_package/cotire.cmake
            auto test_cotire_path = test_package_dir / "cotire.cmake";
            std::ofstream test_cotire_ofs{test_cotire_path};
            test_cotire_ofs << cotire_cmake;
        }

        if (args["--header_only"].asBool()) {
            {  // write conanfile.py
                std::ofstream conanfile_py_ofs{conanfile_py_path};
                conanfile_py_ofs << conanfile_py_header_only(
                    proj_name, opt_author_name, opt_author_email,
                    opt_github_username, opt_topic, opt_description);
            }
            // no need for cmake file, since there is no build
            {  // write src/proj_name/proj_name.h
                auto hello_header_path =
                    namespace_dir / fmt::format("{}.h", proj_name);
                std::ofstream hello_header_ofs{hello_header_path};
                hello_header_ofs << hello_header_only;
            }
        } else {
            {  // write conanfile.py
                std::ofstream conanfile_py_ofs{conanfile_py_path};
                conanfile_py_ofs << conanfile_py_lib(
                    proj_name, opt_author_name, opt_author_email,
                    opt_github_username, opt_topic, opt_description);
            }
            {  // write src/CmakeLists.txt
                std::ofstream cmakelists_ofs{cmakelists_path};
                cmakelists_ofs
                    << cmakelists_render(proj_name, ProjType::library);
            }
            {  // write sr/proj_name/proj_name.h
                auto lib_header_path =
                    namespace_dir / fmt::format("{}.h", proj_name);
                std::ofstream lib_header_ofs{lib_header_path};
                lib_header_ofs << hello_lib_h;
            }
            {  // write sr/proj_name/proj_name.cpp
                auto lib_cpp_path =
                    namespace_dir / fmt::format("{}.cpp", proj_name);
                std::ofstream lib_cpp_ofs{lib_cpp_path};
                lib_cpp_ofs << hello_lib_cpp;
            }
        }
    }
    return 0;
}