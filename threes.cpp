/**
 * Basic Environment for Game 2048
 * use 'g++ -std=c++11 -O3 -g -o 2048 2048.cpp' to compile the source
 *
 * Computer Games and Intelligence (CGI) Lab, NCTU, Taiwan
 * http://www.aigames.nctu.edu.tw
 */

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <ctime>
#include "board.h"
#include "action.h"
#include "agent.h"
#include "episode.h"
#include "statistic.h"

#include "flags/include/flags.h"

int main(int argc, char** argv) {
    const flags::args args(argc, argv);

    std::cout << "THREES-AI: ";
    std::cout << std::endl << std::endl;

    auto total = args.get<size_t>("total", 1000);
    auto block = args.get<size_t>("block", 0);
    auto limit = args.get<size_t>("limit", 0);
    auto play_args = args.get<std::string>("play", "");
    auto evil_args = args.get<std::string>("evil", "");
    auto load = args.get<std::string>("load", "");
    auto save = args.get<std::string>("save", "");
    auto summary = args.get<bool>("summary", false);
    auto alpha = args.get<float>("alpha", 0.05);
    auto feature = args.get<int>("feature", 0);

    statistic stat(total, block, limit);

    if (load.size()) {
        std::ifstream in(load, std::ios::in);
        in >> stat;
        in.close();
        summary |= stat.is_finished();
    }

    weight v4_8(alpha, {{{0, 1, 2, 3},
                         {3, 7, 11, 15},
                         {15, 14, 13, 12},
                         {12, 8, 4, 0}},
                        {{1, 5, 9, 13},
                         {2, 6, 10, 14},
                         {4, 5, 6, 7},
                         {8, 9, 10, 11}}});
    weight v6_32(alpha, {{{0, 1, 2, 3, 4, 5},
                          {3, 2, 1, 0, 7, 6},
                          {12, 13, 14, 15, 8, 9},
                          {15, 14, 13, 12, 11, 10},
                          {0, 4, 8, 12, 1, 5},
                          {12, 8, 4, 0, 13, 9},
                          {3, 7, 11, 15, 2, 6},
                          {15, 11, 7, 3, 14, 10}},
                         {{4, 5, 6, 7, 8, 9},
                          {7, 6, 5, 4, 11, 10},
                          {8, 9, 10, 11, 4, 5},
                          {11, 10, 9, 8, 7, 6},
                          {1, 5, 9, 13, 2, 6},
                          {13, 9, 5, 1, 14, 10},
                          {2, 6, 10, 14, 1, 5},
                          {14, 10, 6, 2, 13, 9}},
                         {{9, 10, 11, 13, 14, 15},
                          {10, 9, 8, 14, 13, 12},
                          {5, 6, 7, 1, 2, 3},
                          {6, 5, 4, 2, 1, 0},
                          {6, 10, 14, 7, 11, 15},
                          {10, 6, 2, 11, 7, 3},
                          {5, 9, 13, 4, 8, 12},
                          {9, 5, 1, 8, 4, 0}},
                         {{5, 6, 7, 9, 10, 11},
                          {9, 10, 11, 5, 6, 7},
                          {6, 5, 4, 10, 9, 8},
                          {10, 9, 8, 6, 5, 4},
                          {9, 5, 1, 10, 6, 2},
                          {10, 6, 2, 9, 5, 1},
                          {5, 9, 13, 6, 10, 14},
                          {6, 10, 14, 5, 9, 13}}});
    weight v[2] = {v4_8, v6_32};
    player play(play_args, v[feature]);
    rndenv evil(evil_args + std::string("seed=") + std::to_string(int(time(0))));

    while (!stat.is_finished()) {
        play.open_episode("~:" + evil.name());
        evil.open_episode(play.name() + ":~");

        stat.open_episode(play.name() + ":" + evil.name());
        episode& game = stat.back();

        for(int i = 0 ; i < 9; i++) {
            game.reset_time();
            action move = evil.take_action(game.state(), game.last_action());
            game.apply_action(move);
        }
        while (true) {
            action move;
           
            game.reset_time();
            move = play.take_action(game.state(), game.last_action());
            if (!game.apply_action(move)) break;

            board s = game.state();
            
            game.reset_time();
            move = evil.take_action(game.state(), game.last_action());
            if (!game.apply_action(move)) break;
            
            board ss = game.state();
            
            play.learn(s, ss);
        }

        agent& win = game.last_turns(play, evil);
        stat.close_episode(win.name());

        play.close_episode(win.name());
        evil.close_episode(win.name());
    }

    play.save_weight();

    if (summary) {
        stat.summary();
    }

    if (save.size()) {
        std::ofstream out(save, std::ios::out | std::ios::trunc);
        out << stat;
        out.close();
    }

    return 0;
}
