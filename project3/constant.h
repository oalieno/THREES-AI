#pragma once

#include <tuple>
#include <vector>

using namespace std;

const int UP = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int LEFT = 3;

const tuple<vector<int>, int> SLIDES[4] = {{{3, 4, 5}, -3},
                                           {{1, 0, 4, 3}, 1},
                                           {{0, 1, 2}, 3},
                                           {{1, 2, 4, 5}, -1}};

const vector<int> GENERATE[4] = {{3, 4, 5},
                                 {0, 3},
                                 {0, 1, 2},
                                 {2, 5}};

const vector<int> BAG[6] = {{1, 2, 3},
                            {2, 3},
                            {1, 3},
                            {3},
                            {2},
                            {1}};

const tuple<vector<int>, vector<int>, int> LAST[4] = {{{0, 1, 2}, {3, 0}, 2},
                                                      {{0, 3}, {0, 1, 2}, 3},
                                                      {{0, 1, 2}, {0, 3}, 2},
                                                      {{0, 3}, {2, 1, 0}, 3}};
