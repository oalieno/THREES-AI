#pragma once

#include <vector>

#include "action.h"
#include "bag.h"
#include "board.h"

struct Environment {
    std::string name;
    int bonus, least48;
    Bag bag;

    Environment () : name("CyKOR"), bonus(0), least48(0) {}

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

    int generateValue (const Board& board) {
        int maxTile = board.maxTile();
        if (maxTile >= 7) {
            least48++;
            if (((float)(bonus + 1)) / least48 < 1.0 / 21.0) {
                bonus++;
                return 4 + rand() % (maxTile - 6);
            }
        }
        return bag.get();
    }

    Action::Place* move (const Board& board, Action::Slide* last) {
        int index = randomIndex(board, APPEARINDEXES[last->direction]);
        int value = board.hint;
        int hint = generateValue(board);
        return ACTIONPLACES[index][value][hint];
    }
};
