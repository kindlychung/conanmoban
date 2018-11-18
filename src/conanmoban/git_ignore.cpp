#include <string>

std::string git_ignore = R"###(
/buck-out/
/.buckd/
/buckaroo/
.buckconfig.local
BUCKAROO_DEPS
**/build
**/*exe
)###";
