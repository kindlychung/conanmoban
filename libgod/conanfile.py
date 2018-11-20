
from conans import ConanFile, CMake, tools
from pathlib import Path
import os

class libgodConan(ConanFile):
    name = "libgod"
    version = "0.0.1"
    license = "LGPL"
    author = "kaiyin keenzhong@qq.com"
    url = "https://github.com/kindlychung/libgod"
    description = "change_your_description"
    topics = ("cpp", )
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    requires = ("docopt/0.6.2@conan/stable",) 
    generators = "cmake"
    exports_sources = "src/*"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()

    def package(self):
        self.copy("libgod/*.hpp", dst="include", src="src")
        self.copy("libgod/*.h", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)


    def imports(self):
        self.copy("*", dst="include", src="include")
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.a", dst="bin", src="lib")
        self.copy("*.so", dst="bin", src="lib")

    def package_info(self):
        self.cpp_info.libs = ["libgod"]
