#pragma once

#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <experimental/filesystem>

#include "spdlog/spdlog.h"
#include "fmt/format.h"

#include "constant.h"
#include "ntuple.h"
#include "utils.h"

namespace fs = std::experimental::filesystem;

struct Weight {
    float alpha;
    std::string path;
    NTuple ntuple;
    float **lut;

    Weight () {}
    Weight (const NTuple& ntuple, float alpha, std::string directory, std::string filename) : ntuple(ntuple), alpha(alpha) {
        checkDirectory(directory);
        path = fmt::format("{}/{}", directory, filename);
        lut = lutMemoryCopy(NULL);
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
        path = weight.path;
        ntuple = weight.ntuple;
        lut = lutMemoryCopy(weight.lut);
        return *this;
    }

    static std::string recommendedFileName (const NTuple& ntuple, float alpha) {
        return fmt::format("{}-{:.10f}", ntuple.name, alpha);
    }

    void checkDirectory (std::string directory) {
        fs::path dp = directory;
        if (not fs::exists(dp)) {
            if (create_directory(dp)) {
                spdlog::info("{} directory created successfully", bold(directory));
            } else {
                spdlog::error("{} directory failed to create", bold(directory));
                exit(0);
            }
        } else if (not fs::is_directory(dp)) {
            spdlog::info("{} directory already exists but is not a directory", bold(directory));
            exit(0);
        }
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
        fs::path p = path;
        if (not fs::exists(p)) {
            spdlog::info("weight {} does not exist, therefore is not loaded", bold(path));
            return;
        }

        std::ifstream file(path);
        if (file) {
            int l = hashSize(ntuple);
            for (int i = 0; i < ntuple.type; i++) {
                file.read((char*)lut[i], sizeof(float) * l);
            }
        }
        file.close();

        spdlog::info("weight {} successfully being loaded", bold(path));
    }

    void dump () {
        std::ofstream file(path);
        for (int i = 0; i < ntuple.type; i++) {
            int l = hashSize(ntuple);
            file.write((char*)lut[i], sizeof(float) * l);
        }
        file.close();
        
        spdlog::info("weight {} successfully being saved", bold(path));
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
