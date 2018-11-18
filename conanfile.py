from conans import ConanFile, CMake

from pathlib import Path
import os
homedir = Path.home()
conan_dir = os.path.join(homedir, ".conan")
conan_bin_dir = os.path.join(conan_dir, "bin")
if not os.path.exists(conan_bin_dir):
    os.mkdir(conan_bin_dir)


class ConanmobanConan(ConanFile):
    name = "conanmoban"
    version = "0.0.1"
    license = "LGPL"
    author = "kaiyin keenzhong@qq.com"
    url = "https://github.com/kindlychung/conanmoban"
    description = "Generate conan project template"
    topics = ("package-management",)
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    requires = "rapidjson/1.1.0@bincrafters/stable", "docopt/0.6.2@conan/stable", "inja/1.0.999@jzien/dev", "fmt/5.2.1@bincrafters/stable"
    generators = "cmake"
    exports_sources = "src/*"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()
        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*", dst="bin", src="bin")

    def imports(self):
        self.copy("*.h", dst="include")
        self.copy("*.hpp", dst="include")
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.a", dst="bin", src="lib")
        self.copy("*.so", dst="bin", src="lib")

    def deploy(self):
        self.copy("*", src="bin", dst=conan_bin_dir)
