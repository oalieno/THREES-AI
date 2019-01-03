#include "../flags/include/flags.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "environment.h"
#include "game.h"
#include "player.h"
#include "constant.h"

using namespace std;

int main (int argc, char** argv) {
    srand(time(0));

    const flags::args args(argc, argv);

    auto total = args.get<int>("total", 1000);
    auto train = args.get<bool>("train", false);
    auto directory = args.get<std::string>("directory", "weights/");
    auto ntuple = args.get<std::string>("ntuple", "6-tuples");
    auto save = args.get<std::string>("save", "");
    
    Weight weight;
    if (NTUPLES.find(ntuple) != NTUPLES.end()) {
        weight = Weight(NTUPLES.at(ntuple), directory);
    } else {
        std::cerr << ntuple << " not found !" << std::endl;
        return -1;
    }


    Environment environment;
    Player player(weight);
    Game game(environment, player);

    game.player.weight.load();
    while (total--) game.play(train);
    if (train) game.player.weight.dump();

    game.stats.summary();

    if (not std::empty(save)) game.stats.save(save);

    return 0;
}
