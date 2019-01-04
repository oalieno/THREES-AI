#pragma once

#include <tuple>
#include <vector>
#include <string>
#include <map>

#include "action.h"
#include "ntuple.h"

const int LENMAX = 15;

const std::tuple<std::vector<int>, int> SLIDES[4] = {{{4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, -4},
                                                    {{2, 6, 10, 14, 1, 5, 9, 13, 0, 4, 8, 12}, 1},
                                                    {{8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3}, 4},
                                                    {{1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15}, -1}};

const std::vector<int> APPEARINDEXES[5] = {{12, 13, 14, 15},
                                           {0, 4, 8, 12},
                                           {0, 1, 2, 3},
                                           {3, 7, 11, 15},
                                           {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};

const std::map<std::string, NTuple> NTUPLES = {
    {
        "4-tuples",
        NTuple(
            "4-tuples",
            {
                {{0, 1, 2, 3}, {3, 7, 11, 15}, {12, 13, 14, 15}, {0, 4, 8, 12}},
                {{1, 5, 9, 13}, {2, 6, 10, 14}, {4, 5, 6, 7}, {8, 9, 10, 11}}
            }
        )
    },
    {
        "6-tuples",
        NTuple(
            "6-tuples",
            {
                {{0, 1, 2, 3, 4, 5}, {3, 2, 1, 0, 7, 6}, {12, 13, 14, 15, 8, 9}, {15, 14, 13, 12, 11, 10}, {0, 4, 8, 12, 1, 5}, {12, 8, 4, 0, 13, 9}, {3, 7, 11, 15, 2, 6}, {15, 11, 7, 3, 14, 10}},
                {{4, 5, 6, 7, 8, 9}, {7, 6, 5, 4, 11, 10}, {8, 9, 10, 11, 4, 5}, {11, 10, 9, 8, 7, 6}, {1, 5, 9, 13, 2, 6}, {13, 9, 5, 1, 14, 10}, {2, 6, 10, 14, 1, 5}, {14, 10, 6, 2, 13, 9}},
                {{9, 10, 11, 13, 14, 15}, {10, 9, 8, 14, 13, 12}, {5, 6, 7, 1, 2, 3}, {6, 5, 4, 2, 1, 0}, {6, 10, 14, 7, 11, 15}, {10, 6, 2, 11, 7, 3}, {5, 9, 13, 4, 8, 12}, {9, 5, 1, 8, 4, 0}},
                {{5, 6, 7, 9, 10, 11}, {9, 10, 11, 5, 6, 7}, {6, 5, 4, 10, 9, 8}, {10, 9, 8, 6, 5, 4}, {9, 5, 1, 10, 6, 2}, {10, 6, 2, 9, 5, 1}, {5, 9, 13, 6, 10, 14}, {6, 10, 14, 5, 9, 13}}
            }
        )
    }
};
