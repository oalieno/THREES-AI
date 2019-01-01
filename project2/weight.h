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
#include <cstring>

class weight {
public:
    typedef std::vector<std::vector<std::vector<int>>> I3;
    weight(int len_max, int feature, const I3& index) : feature(feature), type(size(index)), amount(size(index[0])), len(size(index[0][0])), len_max(len_max), index(index)
    {
        alpha = 0.1 / (type * amount);

        lut = new float*[type];
        for (int i = 0; i < type; i++) {
            int l = power(len_max, len);
            lut[i] = new float[l];
            memset(lut[i], 0, sizeof(float) * l);
        }

        std::stringstream ss;
        ss << std::fixed << std::setprecision(10) << "weights/weight-" << feature << "-" << type << "-" << amount << "-" << len << "-" << len_max << "-" << alpha;
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
            int l = power(len_max, len);
            for (int i = 0; i < type; i++) {
                file.read((char*)lut[i], sizeof(float) * l);
            }
        }
        file.close();
    }
    void save() {
        std::ofstream file(filename);
        for(int i = 0; i < type; i++) {
            int l = power(len_max, len);
            file.write((char*)lut[i], sizeof(float) * l);
        }
        file.close();
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
        return value(s);
    }
    float value(const board& s) {
        float sum = 0.0;
        for (int i = 0; i < type; i++) for (int j = 0; j < amount; j++) {
            sum += f(s, i, j);
        }
        return sum;
    }
    void update(int r, const board& s, const board& ss) {
        auto td_target = (r == -1) ? 0 : r + value(ss);
        auto delta = alpha * (td_target - value(s));
        for (int i = 0; i < type; i++) for (int j = 0; j < amount; j++) {
            f(s, i, j) += delta;
        }
    }

protected:
    float alpha;
    int feature, type, amount, len, len_max;
    std::string filename;
    I3 index;  // [type][amount][len]
    float **lut; // [type][15 ** len]
};
