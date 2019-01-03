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
    int index, value;

    Place () {}
    Place (int index, int value) : index(index), value(value) {}

    void apply (Board& board) override {
        board[index] = value;
    }
    
    void print (std::ostream& out) const override {
        out << std::hex << std::uppercase << index << value;
    }
};

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