#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

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

    for (int i = 0; i < 16; i++) for (int j = 0; j < 15; j++) for (int k = 0; k < 15; k++) {
        ACTIONPLACES[i][j][k] = new Action::Place(i, j, k);
    }

    for (int i = 0; i < 4; i++) {
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

    Environment environment;
    Player player(weight);
    Game game(environment, player);

    game.player.weight.load();
    while (total--) game.play(train);
    if (train) game.player.weight.dump();

    game.stats.summary(std::cerr);

    if (not std::empty(save)) game.stats.save(save);

    return 0;
}
