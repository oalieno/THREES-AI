#pragma once

#include <fstream>
#include <vector>

#include "action.h"
#include "board.h"

struct Stats {
    std::stringstream content;
    int sum, max, tot;

    Stats () : sum(0), max(0), tot(0) {}

    void record (const Board& board, const std::vector<Action*>& moves) {
        content << "td0:random@1540911382577|";
        for (auto move : moves) content << *move;
        content << "|random@1540911382578";
        content << std::endl;
        
        auto score = board.score();
        tot += 1;
        sum += score;
        max = std::max(max, score);
    }

    void save (std::string filename) {
        std::ofstream file(filename);
        file << content.rdbuf();
    }

    void summary () {
        std::cout << "total = " << tot << ", ";
        std::cout << "avg = " << sum / tot << ", ";
        std::cout << "max = " << max;
        std::cout << std::endl;
    }
};
