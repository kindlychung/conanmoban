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

#define VERSION "0.0.1"

// todo: create templates for header only lib
// todo: create templates for a non-header-only lib
// todo: make conanmoban lib --headeronly work
// todo: make conanmoban lib work

// std::string proj_name, std::string author_name, std::string author_email,
// std::string github_username, std::string topic, std::string description);

static const char USAGE[] =
    R"####(Conan project templates. (c) 2018 Kaiyin Zhong

Usage:
   conanmoban bin <proj_name> [--author_name=<author_name>] [--author_email=<author_email>] [--github_username=<github_username>] [--topic=<topic>] [--description=<description>]
   conanmoban lib <proj_name> [--author_name=<author_name>] [--author_email=<author_email>] [--github_username=<github_username>] [--topic=<topic>] [--description=<description>] [--header_only]
   conanmoban (-h | --help)
   conanmoban --version

Options:
  -h --help                               Show this screen.
  --version                               Show version.
  --author_name=<author_name>             Name of the author.
  --author_email=<author_email>           Email of the author.
  --github_username=<github_username>     Github handle.
  --topic=<topic>                         Topic of the project
  --description=<description>             Description of the project.
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
    auto build_dir = src_dir / "build";
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
    std::ofstream git_attr_ofs{git_attr_path};
    git_attr_ofs << git_attr;
    std::ofstream git_ignore_ofs{git_ignore_path};
    git_ignore_ofs << git_ignore;
    std::ofstream cotire_ofs{cotire_path};
    cotire_ofs << cotire_cmake;

    if (args["bin"].asBool()) {
        std::ofstream conanfile_py_ofs{conanfile_py_path};
        conanfile_py_ofs << conanfile_py_binary_render(
            proj_name, opt_author_name, opt_author_email, opt_github_username,
            opt_topic, opt_description);
        std::ofstream cmakelists_ofs{cmakelists_path};
        cmakelists_ofs << cmakelists_binary_render(proj_name);
        auto hello_world_cpp_path =
            namespace_dir / fmt::format("{}.cpp", proj_name);
        std::ofstream hello_world_cpp_ofs{hello_world_cpp_path};
        hello_world_cpp_ofs << hello_world_cpp;
    } else if (args["lib"].asBool()) {
    }
    return 0;
}