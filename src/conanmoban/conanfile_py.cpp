#include <inja.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include "add_data.h"

using json = nlohmann::json;

std::string conanfile_py_imports = R"###(
from conans import ConanFile, CMake, tools, os
)###";

std::string conanfile_py_bin_dir = R"###(
from pathlib import Path
homedir = Path.home()
conan_dir = os.path.join(homedir, ".conan")
conan_bin_dir = os.path.join(conan_dir, "bin")
if not os.path.exists(conan_bin_dir):
    os.mkdir(conan_bin_dir)
)###";

std::string conanfile_py_project_info_test_package = R"###(
class {{ proj_name }}TestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
)###";

std::string conanfile_py_project_info = R"###(
class {{ proj_name }}Conan(ConanFile):
    name = "{{ proj_name }}"
    version = "0.0.1"
    license = "LGPL"
    author = "{{ author_name }} {{ author_email }}"
    url = "https://github.com/{{ github_username }}/{{ proj_name }}"
    description = "{{ description }}"
    topics = ("{{ topic }}", )
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    requires = ("docopt/0.6.2@conan/stable",) 
    generators = "cmake"
    exports_sources = "src/*"
)###";

std::string conanfile_py_build_method = R"###(
    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()
        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)
)###";

std::string conanfile_py_package_method_bin = R"###(
    def package(self):
        self.copy("*", dst="bin", src="bin")
)###";

std::string conanfile_py_package_method_header_only = R"###(
    def package(self):
        self.copy("{{ proj_name }}", dst="include", src="src")
)###";

std::string conanfile_py_imports_method = R"###(
    def imports(self):
        self.copy("*", dst="include", src="include")
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.a", dst="bin", src="lib")
        self.copy("*.so", dst="bin", src="lib")
)###";

std::string conanfile_py_deploy_method_bin = R"###(
    def deploy(self):
        self.copy("*", src="bin", dst=conan_bin_dir)
)###";

std::string conanfile_py_binary_render(
    std::string proj_name, std::string author_name, std::string author_email,
    std::string github_username, std::string topic, std::string description) {
    json data;
    AddData(proj_name);
    AddData(author_name);
    AddData(author_email);
    AddData(github_username);
    AddData(description);
    AddData(topic);
    return conanfile_py_imports + conanfile_py_bin_dir +
           inja::render(conanfile_py_project_info, data) +
           conanfile_py_build_method + conanfile_py_package_method_bin +
           conanfile_py_imports_method + conanfile_py_deploy_method_bin;
}

std::string conanfile_py_test_method = R"###(
    def test(self):
        if not tools.cross_building(self.settings):
            os.chdir("bin")
            self.run(".%sexample" % os.sep)
)###";

std::string conanfile_py_header_only(
    std::string proj_name, std::string author_name, std::string author_email,
    std::string github_username, std::string topic, std::string description) {
    json data;
    AddData(proj_name);
    AddData(author_name);
    AddData(author_email);
    AddData(github_username);
    AddData(description);
    AddData(topic);
    return conanfile_py_imports +
           inja::render(conanfile_py_project_info, data) +
           inja::render(conanfile_py_package_method_header_only, data) +
           conanfile_py_imports_method;
}

std::string conanfile_py_test_package(std::string proj_name) {
    json data;
    AddData(proj_name);
    return conanfile_py_imports +
           inja::render(conanfile_py_project_info_test_package, data) +
           conanfile_py_build_method + conanfile_py_imports_method +
           conanfile_py_test_method;
}