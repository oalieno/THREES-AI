#include <array>
#include <iostream>

#include "state.h"
#include "solver.h"
#include "result.h"
#include "../flags/include/flags.h"

using namespace std;

int main (int argc, char** argv) {
    const flags::args args(argc, argv);

    Solver solver;
    State state;

    while (cin >> state) {
        if (state.type == AFTERSTATE) {
            state.last = state.board.possibleLastAction();
            if (state.last == -1) {
                cout << state << " = " <<  -1 << endl;
                continue;
            }
        }
        cout << state << " = " << solver.lookup(state) << endl;
    }

    return 0;
}
