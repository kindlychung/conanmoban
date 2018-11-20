
from conans import ConanFile, CMake, tools
from pathlib import Path
import os

class headergodConan(ConanFile):
    name = "headergod"
    version = "0.0.1"
    license = "LGPL"
    author = "kaiyin keenzhong@qq.com"
    url = "https://github.com/kindlychung/headergod"
    description = "change_your_description"
    topics = ("cpp", )
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    requires = ("docopt/0.6.2@conan/stable",) 
    generators = "cmake"
    exports_sources = "src/*"

    def package(self):
        self.copy("headergod/*", dst="include", src="src")
    no_copy_source = True
    def imports(self):
        self.copy("*", dst="include", src="include")
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.a", dst="bin", src="lib")
        self.copy("*.so", dst="bin", src="lib")
