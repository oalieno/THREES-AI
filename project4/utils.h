#pragma once

#include <functional>

int power (int x, int m) {
    int ans = 1;
    for (int i = 0; i < m; i++) ans *= x;
    return ans;
}

int argmax (const std::function<float(int)> &f, int x, int y) {
    int a = x;
    float max = f(x);
    for (int i = x + 1; i < y; i++) {        
        if (float r = f(i); r > max) {
            max = r;
            a = i;
        }
    }
    return a;
}
