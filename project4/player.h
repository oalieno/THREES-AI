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
        return [&](int i) -> float { return evaluate(board, i); };
    }

    Action::Slide move (const Board& board) const {
		return Action::Slide(argmax(evaluation(board), 0, 4));   
    }

	void learn (const Board& as0, const Board& bs1) { 
		Board as1 = bs1;
        int action = argmax(evaluation(as1), 0, 4);
        int slidable = as1.slidable(action);
        int r1 = as1.slide(action);     
		weight.update(r1, as0, as1, slidable);
	}
};
