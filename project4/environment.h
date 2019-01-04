#pragma once

#include <vector>

#include "action.h"
#include "bag.h"
#include "board.h"

struct Environment {
    Bag bag;
    int bonus, least48;

    Environment () : bonus(0), least48(0) {}

    int randomIndex (const Board& board, const std::vector<int>& indexes) const {
        int valid = 0;
        for (auto index : indexes) {
            if (board[index] == 0) valid++;
        }
        int counter = rand() % valid;
        for (auto index : indexes) {
            if (board[index] == 0) {
                if (counter == 0) return index;
                counter--;
            }
        }
        return 0;
    }

    Action::Place* move (const Board& board, Action::Slide* last) {
        int index = randomIndex(board, APPEARINDEXES[last->direction]);
        int value = -1;
        int maxTile = board.maxTile();
        if (maxTile >= 7) {
            least48++;
            if (((float)(bonus + 1)) / least48 < 1.0 / 21.0) {
                bonus++;
                value = 4 + rand() % (maxTile - 6);
            }
        }
        if (value == -1) value = bag.get();
        return ACTIONPLACES[index][value];
    }
};
