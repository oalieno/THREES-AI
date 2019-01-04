#pragma once

#include <vector>

#include "action.h"
#include "board.h"
#include "environment.h"
#include "stats.h"
#include "player.h"
#include "weight.h"

struct Game {
    Environment environment;
    Player player;
    Stats stats;
    
    Game () {}
    Game (const Environment& environment, const Player& player) : environment(environment), player(player) {}
    
    void play (bool train) {
        Board board;
        std::vector<Action*> moves;

        environment.bag.clear();

        Action::Slide null(4);
        for (int i = 0; i < 9; i++) {
            Action::Place* place = environment.move(board, &null);
            place->apply(board);
            moves.push_back(place);
        }

        while (not board.dead()) {
            Board as0, bs1;

            Action::Slide* slide = player.move(board);
            slide->apply(board);
            moves.push_back(slide);
            as0 = board;
            
            Action::Place* place = environment.move(board, slide);
            place->apply(board);
            moves.push_back(place);
            bs1 = board;

            if (train) player.learn(as0, bs1);
        }

        stats.record(board, moves);
    }
};
