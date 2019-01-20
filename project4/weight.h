#pragma once

#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>

#include "constant.h"
#include "ntuple.h"

struct Weight {
    float alpha;
    std::string filename;
    NTuple ntuple;
    float **lut;

    Weight () {}
    Weight (const NTuple& ntuple, std::string directory) : ntuple(ntuple) {
        alpha = ntuple.recommendedAlpha();
       
        lut = lutMemoryCopy(NULL);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(10) << directory << ntuple.name << "-" << alpha;
        ss >> filename;
    }
    Weight (const Weight &weight) {
        *this = weight;
    }
    ~Weight () {
        for (int i = 0; i < ntuple.type; i++) delete lut[i];
        delete lut;
    }

    Weight& operator = (const Weight &weight) {
        alpha = weight.alpha;
        filename = weight.filename;
        ntuple = weight.ntuple;
        lut = lutMemoryCopy(weight.lut);
        return *this;
    }

    int hashSize (NTuple ntuple) {
        return 4 * power(LENMAX, ntuple.len);
    }

    float** lutMemoryCopy (float** memory) {
        float** tmp = new float*[ntuple.type];
        for (int i = 0; i < ntuple.type; i++) {
            int l = hashSize(ntuple);
            tmp[i] = new float[l];
            if (memory) memcpy(tmp[i], memory[i], sizeof(float) * l);
            else memset(tmp[i], 0, sizeof(float) * l);
        }
        return tmp;
    }

    void load () {
        std::ifstream file(filename);
        if (file) {
            int l = hashSize(ntuple);
            for (int i = 0; i < ntuple.type; i++) {
                file.read((char*)lut[i], sizeof(float) * l);
            }
        }
        file.close();
    }

    void dump () {
        std::ofstream file(filename);
        for (int i = 0; i < ntuple.type; i++) {
            int l = hashSize(ntuple);
            file.write((char*)lut[i], sizeof(float) * l);
        }
        file.close();
    }

    float value (const Board& board) const {
        float sum = 0.0;
        for (int i = 0; i < ntuple.type; i++) {
            for (int j = 0; j < ntuple.isomorphism; j++) {
                sum += lutGet(board, i, j);
            }
        }
        return sum;
    }

    float& lutGet (const Board& board, int type, int isomorphism) const {
        int hash = 0;
        for (auto index : ntuple(type, isomorphism)) {
            hash = hash * LENMAX + board[index];
        }
        hash = 4 * hash + (board.hint > 3 ? 0 : board.hint);
        return lut[type][hash];
    }

    void update (int r1, const Board& as0, const Board& as1, bool slidable) {
        auto target = slidable ? r1 + value(as1) : 0;
        auto delta = alpha * (target - value(as0));
        for (int i = 0; i < ntuple.type; i++) {
            for (int j = 0; j < ntuple.isomorphism; j++) {
                lutGet(as0, i, j) += delta;
            }
        }
    }

    float operator () (const Board& board) const {
        return value(board);
    }
};
