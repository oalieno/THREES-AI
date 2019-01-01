#pragma once

#include <vector>

#include "action.h"
#include "bag.h"
#include "board.h"

struct Environment {
    Bag bag;

    Environment () {}

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

    Action::Place move (const Board& board, const Action::Slide& last) {
        int index = randomIndex(board, APPEARINDEXES[last.direction]);
        int value = bag.get();
        return Action::Place(index, value);
    }
};
