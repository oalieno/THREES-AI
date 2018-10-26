/**
 * Framework for 2048 & 2048-like Games (C++ 11)
 * use 'g++ -std=c++11 -O3 -g -o 2048 2048.cpp' to compile the source
 *
 * Author: Hung Guei (moporgic)
 *         Computer Games and Intelligence (CGI) Lab, NCTU, Taiwan
 *         http://www.aigames.nctu.edu.tw
 */

#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <fstream>

class weight {
public:
    weight() : filename("weight.txt"), index({{{0, 1, 2, 3}, 
                                              {3, 7, 11, 15},
                                              {15, 14, 13, 12},
                                              {12, 8, 4, 0},
                                              {1, 5, 9, 13},
                                              {2, 6, 10, 14},
                                              {4, 5, 6, 7},
                                              {8, 9, 10, 11}}}), lut({})
    {
        load();
    }
    void load() {
        std::ifstream file(filename);
        if (file) {
            for (auto& i : lut) for (auto& j : i) file >> j;
        }
        file.close();
    }
    void save() {
        std::ofstream file(filename);
        for (auto& i : lut) for (auto& j : i) file << std::fixed << std::setprecision(20) << j << " ";
    }
    float& f(const board& s, int idx) {
        int key = 0;
        for (auto i : index[idx]) key = key * 15 + s(i);
        return lut[idx > 3][key];
    }
    float operator() (const board& s) {
        float sum = 0.0;
        for (int i = 0; i < 8; i++) {
            sum += f(s, i);
        }
        return sum;
    }
    void update(int r, const board& s, const board& ss) {
        float alpha = 0.005;
        for (int i = 0; i < 8; i++) {
            if (r == -1) f(s, i) += alpha * -f(s, i);
            else f(s, i) += alpha * (r + f(ss, i) - f(s, i));
        }
    }

protected:
    static const int size = 15 * 15 * 15 * 15;
    const char *filename;
    std::array<std::array<int, 4>, 8> index; // [8][4]
    std::array<std::array<float, size>, 2> lut; // [2][15 ** 4]
};
