#pragma once

int i2v(int index) {
    if (index < 3) return index;
    return 3 * (1 << (index - 3));
}
