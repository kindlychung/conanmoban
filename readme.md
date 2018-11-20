# conanmoban: a streamlined commandline tool for generating conan project templates

> Written in C++, for C++.

# Install

Get conan (the c++ package manager) and do the following in the terminal:

```
conan install conanmoban/1.0.0@jzien/dev -r jzien
```

Make sure that `$HOM#/.conan/bin` is in your `$PATH`.

# Usage

See `conanmoban -h`.

# Conventions (aka opinionated simplifications)

* Project layout is shown in the examples `cligod`, `headergod` and `libgod`
* `build` directory is created in project root, outside `src`.
* Use `cotire` to speed up compilation.
* There is always a `test_package` folder for library projects.
* Use `conanfile.py` instead of `conanfile.txt` always.
* CLI project binaries are always deployed to `$HOME/.conan/bin`