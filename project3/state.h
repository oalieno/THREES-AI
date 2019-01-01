#pragma once

#include "board.h"
#include "constant.h"

struct Type {
    bool value;

    Type () {}
    Type (bool value) : value(value) {}

    void change () {
        value = !value;
    }

    bool operator == (const Type& t) const {
        return value == t.value;
    }

    friend istream& operator >> (istream& in, Type& t) {
        char type; in >> type;
        t.value = type == 'a' ? 0 : 1;
        return in;
    }

    friend ostream& operator << (ostream& out, Type& t) {
        out << (t.value == 0 ? "a" : "b");
        return out;
    }
};

const Type AFTERSTATE(0);
const Type BEFORESTATE(1);

struct State {
    Board board;
    Type type;
    int hint, last;

    State () {}
    State (Type type) : type(type) {}

    vector<int> recoverBag () const {
        int s = board.sumTiles() % 6;
        auto bag = BAG[s];
        if (s == 3 and hint == 3) bag = {1, 2};
        return bag;
    }

    friend istream& operator >> (istream& in, State& s) {
        in >> s.type >> s.board >> s.hint;
        return in;
    }

    friend ostream& operator << (ostream& out, State& s) {
        out << s.type << " " << s.board << " +" << s.hint;
        return out;
    }
};
