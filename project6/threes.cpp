#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <regex>

#include "flags.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "environment.h"
#include "game.h"
#include "player.h"
#include "constant.h"
#include "ntuple.h"
#include "weight.h"

void init () {
    srand(time(0));

    for (int i = 0; i < 16; i++) for (int j = 0; j < 15; j++) for (int k = 0; k < 5; k++) {
        ACTIONPLACES[i][j][k] = new Action::Place(i, j, k);
    }

    for (int i = 0; i < 5; i++) {
        ACTIONSLIDES[i] = new Action::Slide(i);
    }

    auto console = spdlog::stderr_color_mt("stderr");
    spdlog::set_default_logger(console);
}

NTuple loadNTuple (std::string ntupleName) {
    if (NTUPLES.find(ntupleName) != NTUPLES.end()) {
        return NTUPLES.at(ntupleName);
    } else {
        spdlog::error("--ntuple={} invalid argument", ntupleName);
        spdlog::error("--ntuple only allow 4-tuples, 6-tuples");
        exit(0);
    }
}

int slideString2Int (std::string str) {
    if (str == "U") return 0;
    else if (str == "R") return 1;
    else if (str == "D") return 2;
    else if (str == "L") return 3;
    else return -1;
}

int main (int argc, char** argv) {
    init();

    const flags::args args(argc, argv);

    auto total = args.get<int>("total", 1000);
    auto train = args.get<bool>("train", false);
    auto save = args.get<std::string>("save", "");
    auto arena = args.get<bool>("arena", false);
    
    auto ntupleName = args.get<std::string>("ntuple", "6-tuples");
    NTuple ntuple = loadNTuple(ntupleName);
    
    auto alpha = args.get<float>("alpha", NTuple::recommendedAlpha(ntuple.type, ntuple.isomorphism));
    auto directory = args.get<std::string>("directory", "weights");
    auto filename = args.get<std::string>("filename", Weight::recommendedFileName(ntuple, alpha));
    Weight weight(ntuple, alpha, directory, filename);

    Environment environment(weight);
    Player player(weight);

    if (arena) {
        bool playing = false;
        bool attack = false;
        Board board;

        player.weight.load();

        Action::Slide* slide;
        Action::Place* place;

        std::regex r_info("^% (.*)$");
        std::regex r_message("^\\? message from (.*): (.*)$");
        std::regex r_open("^#(\\S+) open (\\S+):(\\S+)$");
        std::regex r_close("^#(\\S+) close (\\S+)$");
        std::regex r_ask("^#(\\S+) \\?$");
        std::regex r_pmove("^#(\\S+) #([UDLR])$");
        std::regex r_emove("^#(\\S+) ([0-9A-F])([0-9A-F])\\+([1234])$");
        std::smatch sm;

        std::string cmd;
        while (getline(std::cin, cmd)) {
            if (cmd == "@ login") {
                std::string reply = fmt::format("@ login 0410022|07ssj4N1au {}(player) {}(environment)", player.name, environment.name);
                std::cout << reply << std::endl;
            } else if (std::regex_match(cmd, sm, r_info)) {
                //spdlog::info("arena info | {}", std::string(sm[1]));
            } else if (std::regex_match(cmd, sm, r_message)) {
                //spdlog::info("message from {} | {}", std::string(sm[1]), std::string(sm[2]));
            } else if (std::regex_match(cmd, sm, r_open)) {
                std::string id = sm[1];
                std::string p1 = sm[2];
                std::string p2 = sm[3];
                std::string reply = fmt::format("#{} open {}", id, playing ? "reject" : "accept");
                std::cout << reply << std::endl;
                // init game
                if (playing) continue;
                playing = true;
                board = Board();
                if (p1 == "$0410022" or p1 == "PPP") {
                    attack = true;
                } else {
                    attack = false;
                    environment.clear();
                    slide = ACTIONSLIDES[4];
                }
            } else if (std::regex_match(cmd, sm, r_close)) {
                playing = false;
            } else if (std::regex_match(cmd, sm, r_ask)) {
                Action* action;
                if (attack) {
                    slide = player.move(board);
                    slide->apply(board);
                    action = slide;
                } else {
                    place = environment.move(board, slide, true);
                    place->apply(board);
                    action = place;
                }
                std::string id = sm[1];
                std::cout << "#" << id << " " << *action << std::endl;
            } else if (std::regex_match(cmd, sm, r_pmove)) {
                std::string id = sm[1];
                std::string s = sm[2];
                slide = ACTIONSLIDES[slideString2Int(s)];
                slide->apply(board);
            } else if (std::regex_match(cmd, sm, r_emove)) {
                std::string id = sm[1];
                int index = std::stol(sm[2], nullptr, 16);
                int value = std::stol(sm[3], nullptr, 16);
                int hint = std::stol(sm[4]);
                place = ACTIONPLACES[index][value][hint];
                place->apply(board);
            }
        }
    } else {
        Game game(environment, player);

        game.player.weight.load();
        while (total--) game.play(train);
        if (train) game.player.weight.dump();

        game.stats.summary(std::cerr);

        if (not std::empty(save)) game.stats.save(save);
    }

    return 0;
}
