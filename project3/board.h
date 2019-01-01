#pragma once

#include <array>

#include "constant.h"
#include "utils.h"

using namespace std;

struct Board {
    array<int, 6> value;
    
    Board () {
        for (int i = 0; i < 6; i++) value[i] = 0;
    }
    
    int tileToValue(int tile) const {
        if (tile == 0 or tile % 3 != 0) return tile;
        tile = tile / 3;
        
        int ans = 0;
        while ((1 << ans) < tile) ans++;
        return ans + 3;
    }
    
    int valueToTile(int value) const {
        if (value < 3) return value;
        return 3 * (1 << (value - 3));    
    }

    int merge(int v1, int v2) const {
        return max(v1, v2) + 1;
    }

    bool checkMerge(int v1, int v2) const {
        if (v1 >= 14 or v2 >= 14) return false;
        if (v1 > v2) swap(v1, v2);
        if (v1 == 1 and v2 == 2) return true;
        if (v1 > 2 and v2 > 2 and v1 == v2) return true;
        return false;
    }

    bool slidable (int action) const {
        Board b = *this;
        return *this != b.slide(action);
    }

    Board slide (int action) {
        auto [indexes, offset] = SLIDES[action];
        for (auto index : indexes) {
            auto& v = value[index];
            auto& nv = value[index + offset];
            slideAtom(v, nv);
        }
        return *this;
    }

    void slideAtom (int& v, int& nv) {
        if (v == 0) {
            return;
        } else if (nv == 0) {
            nv = v;
            v = 0;
        } else if (checkMerge(v, nv)) {
            nv = merge(nv, v);
            v = 0;
        }
    }
    
    bool isGameOver () const {
        for (int action = 0; action < 4; action++) {
            if (slidable(action)) return false;
        }
        return true;
    }

    bool lastActionable (int action) {
        bool hasOne = false;
        auto [iRange, jRange, bits] = LAST[action];
        for (auto i : iRange) {
            int v = 0;
            for (auto j : jRange) {
                v = (v << 1) + (value[i + j] > 0);
            }
            if (0 < v and v < (1 << (bits - 1))) hasOne = true;
            else if (v != 0 and v != (1 << bits) - 1) return false;
        }
        return hasOne;
    }

    int possibleLastAction () {
        for (int action = 0; action < 4; action++) {
            if (lastActionable(action)) return action;
        }
        return -1;
    }

    int score () const {
        int sum = 0;
        for (int i = 0; i < 6; i++) {
            if (value[i] >= 3) {
                sum += power(3, value[i] - 2);
            }
        }
        return sum;
    }

    int sumTiles () const {
        int sum = 0;
        for (int i = 0; i < 6; i++) {
            sum += valueToTile(value[i]);
        }
        return sum;
    }

    bool operator == (const Board& b) const {
        for (int i = 0; i < 6; i++) {
            if (value[i] != b.value[i]) return false;
        }
        return true;
    }
    
    bool operator != (const Board& b) const {
        return !(*this == b);
    }

    int& operator [] (int index) {
        return value[index];
    }
    
    const int& operator [] (int index) const {
        return value[index];
    }

    friend istream& operator >> (istream& in, Board& b) {
        for (int i = 0; i < 6; i++) {
            int tile; in >> tile;
            b.value[i] = b.tileToValue(tile);
        }
        return in;
    }

    friend ostream& operator << (ostream& out, Board& b) {
        for (int i = 0; i < 6; i++) {
            out << b.valueToTile(b.value[i]);
            if (i != 5) out << " ";
        }
        return out;
    }
};
