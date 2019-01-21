#pragma once

#include <vector>

struct Bag {
    Bag () {}

    void clear () {
        value.clear();
        fill();
    }

    void fill () {
        for (int k = 0; k < 4; k++) {
            for (int i = 1; i <= 3; i++) {
                value.push_back(i);
            }
        }
    }

    int pop (int index) {
        int tile = value[index];
        value.erase(value.begin() + index);
        return tile;
    }

    int get () {
        int tile = pop(rand() % value.size());
        if (value.empty()) fill();
        return tile;
    }

    std::vector<int> value;
};
