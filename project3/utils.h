#pragma once

int power (int x, int m) {
    int ans = 1;
    for (int i = 0; i < m; i++) ans *= x;
    return ans;
}
