#pragma once

#include <array>

#include "constant.h"
#include "utils.h"

struct Board {
    std::array<int, 16> value;
    int hint;

    Board () : value{0} {}

    int tileToValue (int tile) const {
        if (tile == 0 or tile % 3 != 0) return tile;
        tile = tile / 3;
        
        int ans = 0;
        while ((1 << ans) < tile) ans++;
        return ans + 3;
    }
    
    int valueToTile (int value) const {
        if (value < 3) return value;
        return 3 * (1 << (value - 3));    
    }

    int valueToScore (int value) const {
        if (value < 3) return 0;
        return power(3, value - 2);
    }

    int merge (int v1, int v2) const {
        return std::max(v1, v2) + 1;
    }

    bool checkMerge (int v1, int v2) const {
        if (v1 >= 14 or v2 >= 14) return false;
        if (v1 > v2) std::swap(v1, v2);
        if (v1 == 1 and v2 == 2) return true;
        if (v1 > 2 and v2 > 2 and v1 == v2) return true;
        return false;
    }

    bool dead () const {
        for (int action = 0; action < 4; action++) {
            if (slidable(action)) return false;
        }
        return true;
    }

    int score () const {
        int sum = 0;
        for (int i = 0; i < 16; i++) {
            sum += valueToScore(value[i]);
        }
        return sum;
    }

    int maxTile () const {
        int max = 0;
        for (int i = 0; i < 16; i++) {
            max = std::max(max, value[i]);
        }
        return max;
    }

    bool slidable (int action) const {
        Board tmp = *this;
        tmp.slide(action);
        return *this != tmp;
    }

    int slide (int action) {
        auto [indexes, offset] = SLIDES[action];
        int reward = 0;
        for (auto index : indexes) {
            int& v = value[index];
            int& nv = value[index + offset];
            reward += slideAtom(v, nv);
        }
        return reward;
    }

    int slideAtom (int& v, int& nv) {
        int reward = 0;
        if (v == 0) {
            // pass
        } else if (nv == 0) {
            nv = v;
            v = 0;
        } else if (checkMerge(v, nv)) {
            reward -= valueToScore(v) + valueToScore(nv);
            nv = merge(nv, v);
            v = 0;
            reward += 0 + valueToScore(nv);
        }
        return reward;
    }
    
    bool operator == (const Board& board) const {
        for (int i = 0; i < 16; i++) {
            if (value[i] != board.value[i]) return false;
        }
        return true;
    }
    
    bool operator != (const Board& board) const {
        return !(*this == board);
    }

    int& operator [] (int index) { return value[index]; }
    const int& operator [] (int index) const { return value[index]; }
};
