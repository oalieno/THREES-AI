#pragma once

#include <random>

class bag {
public:
    bag() : used({false, false, false}), uniform(0, 5) {}
    int get_tile(std::default_random_engine &engine) {
        if (size == 0) {
            used[0] = used[1] = used[2] = false;
            size = 3;
        }
        int rand = uniform(engine);
        int index = rand % size;
        for(int i = 0; i < 3; i++) {
            if (index == 0) return i + 1;
            if (not used[i]) index--;
        }
        return 0;
    }
private:
    int size;
    std::array<bool, 3> used;
    std::uniform_int_distribution<int> uniform;
};
