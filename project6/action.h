#pragma once

#include "board.h"

struct Action {
    Action () {}

    struct Place;
    struct Slide;

    virtual void apply (Board& board) {}
    virtual void print (std::ostream& o) const {}

    friend std::ostream& operator << (std::ostream& out, const Action& action) {
        action.print(out);
        return out;
    }
};

struct Action::Place : Action {
    int index, value, hint;

    Place () {}
    Place (int index, int value, int hint) : index(index), value(value), hint(hint) {}

    void apply (Board& board) override {
        board[index] = value;
        board.hint = hint;
    }
    
    void print (std::ostream& out) const override {
        out << std::hex << std::uppercase << index << value << "+" << hint;
    }
};

Action::Place* ACTIONPLACES[16][15][5];

struct Action::Slide : Action {
    int direction;

    Slide () {}
    Slide (int direction) : direction(direction) {}

    void apply (Board& board) override {
        board.slide(direction);
    }

    void print (std::ostream& out) const override {
        out << "#" << ("URDL")[direction];
    }
};

Action::Slide* ACTIONSLIDES[5];
