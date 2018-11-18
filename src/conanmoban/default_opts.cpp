#include "default_opts.h"
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <string>
namespace fs = std::filesystem;

std::string default_opts_string() {
    struct passwd const* pw = getpwuid(getuid());
    char const* homedir = pw->pw_dir;
    fs::path const conanmoban_config_path =
        fs::path(homedir) / ".conanmoban.json";
    std::ifstream ifs(conanmoban_config_path);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}

json default_opts() { return json::parse(default_opts_string()); }