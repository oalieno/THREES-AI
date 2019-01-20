#pragma once

#include <vector>
#include <string>

struct NTuple {
    int type, isomorphism, len;
    std::vector<std::vector<std::vector<int>>> index;
    std::string name;

    NTuple () {}
    NTuple (std::string name, const std::vector<std::vector<std::vector<int>>>& index) : index(index), name(name) {
        type = std::size(index);
        isomorphism = std::size(index[0]);
        len = std::size(index[0][0]);
    }

    static float recommendedAlpha (int type, int isomorphism) {
        return 0.1 / (type * isomorphism);
    }

    std::vector<int> operator () (int type, int isomorphism) const {
        return index[type][isomorphism];
    }
};
