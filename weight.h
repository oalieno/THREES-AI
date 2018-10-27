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
#include <string>
#include <sstream>

class weight {
public:
    typedef std::vector<std::vector<std::vector<int>>> I3;
    typedef std::vector<std::vector<float>> F2;
    weight(float alpha, int len_max, const I3& index) : alpha(alpha), type(size(index)), amount(size(index[0])), len(size(index[0][0])), len_max(len_max), index(index)
    {
        for(int i = 0; i < type; i++) {
            lut.push_back(std::vector<float>(power(len_max, len), 0.0));
        }
        std::stringstream ss;
        ss << std::fixed << std::setprecision(10) << "weight-" << type << "-" << amount << "-" << len << "-" << len_max << "-" << alpha;
        ss >> filename;
        load();
    }
    int power(int x, int m) {
        int ans = 1;
        for(int i = 0; i < m; i++) ans *= x;
        return ans;
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
    float& f(const board& s, int i, int j) {
        int key = 0;
        for (auto k : index[i][j]) {
            int sk = s(k);
            key = key * len_max + (sk >= len_max ? len_max - 1 : sk);
        }
        return lut[i][key];
    }
    float operator() (const board& s) {
        float sum = 0.0;
        for (int i = 0; i < type; i++) for (int j = 0; j < amount; j++) {
            sum += f(s, i, j);
        }
        return sum;
    }
    void update(int r, const board& s, const board& ss) {
        for (int i = 0; i < type; i++) for (int j = 0; j < amount; j++) {
            auto td_target = (r == -1) ? 0 : r + f(ss, i, j);
            f(s, i, j) += alpha * (td_target - f(s, i, j));
        }
    }

protected:
    float alpha;
    int type, amount, len, len_max;
    std::string filename;
    I3 index; // [type][amount][len]
    F2 lut;   // [type][15 ** len]
};
