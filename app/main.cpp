#include "Plugin.h"
#include <iostream>

namespace fs = std::filesystem;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Plugin p{"plugins\\funcadd.dll"};
    std::cout << p.call({4,2});
    return 0;
}