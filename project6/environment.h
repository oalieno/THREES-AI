#pragma once

#include <vector>
#include <limits>

#include "action.h"
#include "bag.h"
#include "board.h"
#include "weight.h"

struct Environment {
    std::string name;
    int bonus, least48, hint;
    Bag bag;
    Weight weight;

    Environment () {}
    Environment (const Weight& weight) : name("CyKOR"), bonus(0), least48(0), weight(weight) {
        clear();
    }

    void clear () {
        bag.clear();
        hint = generateValue(0);
    }

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

    bool canBonus (int maxTile) const {
        if (maxTile >= 7 and (bonus + 1.0) / (least48 + 1.0) < 1.0 / 21.0) return true;
        return false;
    }

    int generateValue (int maxTile) {
        if (canBonus(maxTile)) return 4 + rand() % (maxTile - 6);
        return bag.get();
    }

    void updateCounter(int maxTile, int hint) {
        if (maxTile >= 7) {
            least48++;
            if (hint >= 4) {
                bonus++;
            }
        }
    }

    float layer1 (const Board& board) {
        float max = std::numeric_limits<float>::lowest();
        for (int i = 0; i < 4; i++) {
            if (not board.slidable(i)) continue;
            Board tmp = board;
            int reward = tmp.slide(i);
            float score = reward + weight(tmp);
            max = std::max(max, score);
        }
        return max;
    }

    Action::Place* move (const Board& board, Action::Slide* last, bool antagonistic = false) {
        int maxTile = board.maxTile();
        int index, value;
        if (antagonistic) {
            float min = std::numeric_limits<float>::max();
            int index_, hint_;

            for (auto i : APPEARINDEXES[last->direction]) {
                if (board[i] > 0) continue;

                int newhint;
                if (canBonus(maxTile)) {
                    for (int j = 4; j < 4 + (maxTile - 6); j++) newhint = j;
                } else {
                    for (int j = 0; j < bag.value.size(); j++) newhint = bag.value[j];
                }

                Board node = board;
                ACTIONPLACES[i][hint][newhint > 3 ? 4 : newhint]->apply(node);
                float score = node.valueToScore(hint) + layer1(node);
                if (score < min) {
                    min = score;
                    index_ = i;
                    hint_ = newhint;
                }
            }

            index = index_;
            value = hint;
            hint = hint_;
            if (hint <= 3) {
                bag.pop(std::distance(bag.value.begin(), std::find(bag.value.begin(), bag.value.end(), hint)));
                if (bag.value.empty()) bag.fill();
            }
        } else {
            index = randomIndex(board, APPEARINDEXES[last->direction]);
            value = hint;
            hint = generateValue(maxTile);
        }
        updateCounter(maxTile, hint);
        return ACTIONPLACES[index][value][hint > 3 ? 4 : hint];
    }
};
