#pragma once

#include <vector>

struct Bag {
    Bag () {}

    void clear () {
        value.clear();
    }

    int pop (int index) {
        int tile = value[index];
        value.erase(value.begin() + index);
        return tile;
    }

    int get () {
        if (value.empty()) {
            for (int k = 0; k < 4; k++) {
                for (int i = 1; i <= 3; i++) {
                    value.push_back(i);
                }
            }
        }
        return pop(rand() % value.size());
    }

    std::vector<int> value;
};
