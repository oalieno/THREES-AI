#pragma once

#include <limits>

#include "action.h"
#include "utils.h"
#include "weight.h"

struct Player {
    Weight weight;

    Player () {}
    Player (const Weight& weight) : weight(weight) {}
	
	float evaluate (const Board& board, int action) const {  
        Board tmp = board;
        if (not tmp.slidable(action)) return std::numeric_limits<float>::lowest();
        int reward = tmp.slide(action);
        return reward + weight(tmp);
	}
	
    auto evaluation (const Board& board) const {
        int a = 0;
        float max = evaluate(board, 0);
        for (int i = 1; i < 4; i++) {
            if (float r = evaluate(board, i); r > max) {
                max = r;
                a = i;
            }
        }
        return a;
    }

    Action::Slide* move (const Board& board) const {
        return ACTIONSLIDES[evaluation(board)];
    }

	void learn (const Board& as0, const Board& bs1) { 
		Board as1 = bs1;
        int action = evaluation(as1);
        int slidable = as1.slidable(action);
        int r1 = as1.slide(action);     
		weight.update(r1, as0, as1, slidable);
	}
};
