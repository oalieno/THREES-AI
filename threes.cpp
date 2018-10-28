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
#include "utils.h"

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
    auto feature = args.get<int>("feature", 0);
    auto len_max = args.get<int>("maxtile", 15);

    statistic stat(total, block, limit);

    if (load.size()) {
        std::ifstream in(load, std::ios::in);
        in >> stat;
        in.close();
        summary |= stat.is_finished();
    }

    weight v4_8(len_max, w4_8);
    weight v6_32(len_max, w6_32);
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
