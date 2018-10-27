#pragma once

/*
 * index : 0  1  2  3  4  5  6 ...   14
 * value : 0  1  2  3  6 12 24 ... 6144
 */

int i2v(int index) {
    if (index < 3) return index;
    return 3 * (1 << (index - 3));
}

int v2i(int value) {
    if(value % 3 != 0) return value;
    value = value / 3;
    int ans = 0;
    while((1 << ans) < value) ans++;
    return ans + 3;
}


