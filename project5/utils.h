#pragma once

#include "fmt/format.h"

int power (int x, int m) {
    int ans = 1;
    for (int i = 0; i < m; i++) ans *= x;
    return ans;
}

std::string bold(std::string text) {
    return fmt::format("\e[1m{}\e[0m", text);
}
