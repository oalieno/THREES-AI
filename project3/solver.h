#pragma once

#include <map>

#include "constant.h"
#include "result.h"
#include "state.h"

using namespace std;

struct Solver {
    map<int, Result> LUT;

    Solver () {
        for (int i = 0; i < 6; i++) {
            for (int value = 1; value <= 3; value++) {
                for (int hint = 1; hint <= 3; hint++) {
                    if (hint == value) continue;
                    State state(BEFORESTATE);
                    state.board[i] = value;
                    state.hint = hint;
                    solve(state);
                }
            }
        }
    }

    int getKey (const State& state) {
        int key = 0;
        for (int i = 0; i < 6; i++) {
            key = key * 15 + state.board[i];
        }
        key = key * 3 + state.hint;
        int last = 4; if (state.type == AFTERSTATE) last = state.last;
        key = key * 5 + last;
        return key;
    }

    Result solve(const State& state) {
        int key = getKey(state);
        if (LUT.find(key) != LUT.end()) return LUT[key];

        if (state.board.isGameOver()) {
            auto score = state.board.score();
            return LUT[key] = Result(score);
        }

        Result best;
        
        if (state.type == AFTERSTATE) {
            for (auto index : GENERATE[state.last]) {
                if (state.board[index] == 0) {
                    State nextState = state;
                    nextState.board[index] = state.hint;
                    nextState.type.change();
                    for (auto hint : nextState.recoverBag()) {
                        nextState.hint = hint;
                        auto result = solve(nextState);
                        best.improve(state.type, result);
                    }
                }
            }
        } else {
            for (int action = 0; action < 4; action++) {
                if (state.board.slidable(action)) {
                    State nextState = state;
                    nextState.board.slide(action);
                    nextState.last = action;
                    nextState.type.change();
                    auto result = solve(nextState);
                    best.improve(state.type, result);
                }
            }
        }
        
        return LUT[key] = best;
    }

    Result lookup (const State& state) {
        int key = getKey(state);
        if (LUT.find(key) != LUT.end()) return LUT[key];
        return ILLEGAL;
    }
};
