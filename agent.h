#pragma once
#include <string>
#include <random>
#include <sstream>
#include <map>
#include <type_traits>
#include <algorithm>
#include "board.h"
#include "action.h"
#include "bag.h"

class agent {
public:
    agent(const std::string& args = "") {
        std::stringstream ss("name=unknown role=unknown " + args);
        for (std::string pair; ss >> pair; ) {
            std::string key = pair.substr(0, pair.find('='));
            std::string value = pair.substr(pair.find('=') + 1);
            meta[key] = { value };
        }
    }
    virtual ~agent() {}
    virtual void open_episode(const std::string& flag = "") {}
    virtual void close_episode(const std::string& flag = "") {}
    virtual action take_action(const board& b, action last) { return action(); }
    virtual bool check_for_win(const board& b) { return false; }

public:
    virtual std::string property(const std::string& key) const { return meta.at(key); }
    virtual void notify(const std::string& msg) { meta[msg.substr(0, msg.find('='))] = { msg.substr(msg.find('=') + 1) }; }
    virtual std::string name() const { return property("name"); }
    virtual std::string role() const { return property("role"); }

protected:
    typedef std::string key;
    struct value {
        std::string value;
        operator std::string() const { return value; }
        template<typename numeric, typename = typename std::enable_if<std::is_arithmetic<numeric>::value, numeric>::type>
        operator numeric() const { return numeric(std::stod(value)); }
    };
    std::map<key, value> meta;
};

class random_agent : public agent {
public:
    random_agent(const std::string& args = "") : agent(args) {
        if (meta.find("seed") != meta.end())
            engine.seed(int(meta["seed"]));
    }
    virtual ~random_agent() {}

protected:
    std::default_random_engine engine;
};

/**
 * random environment
 * add a new random tile to an empty cell
 * 2-tile: 90%
 * 4-tile: 10%
 */
class rndenv : public random_agent {
public:
    typedef std::vector<int> grid;
    rndenv(const std::string& args = "") : random_agent("name=random role=environment " + args),
        spaces({{ 12, 13, 14, 15 },
                { 0, 4, 8, 12 },
                { 0, 1, 2, 3 },
                { 3, 7, 11, 15 },
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }}) {}

    virtual void open_episode(const std::string& flag = "") {
        popup.clear();
    }

    virtual action take_action(const board& after, action last) {
        grid space = spaces[last.event() & 0b11];
        if ((int)last.code == -1) space = spaces[4];

        std::shuffle(space.begin(), space.end(), engine);
        for (int pos : space) {
            if (after(pos) != 0) continue;
            board::cell tile = popup.get_tile(engine);
            return action::place(pos, tile);
        }
        return action();
    }

private:
    std::vector<grid> spaces;
    bag popup;
};

/**
 * dummy player
 * select a legal action randomly
 */
class player : public random_agent {
public:
    player(const std::string& args = "") : random_agent("name=dummy role=player " + args),
        opcode({ 0, 1, 2, 3 }) {}

    int search(const board& b) {
        int big = -1, op = -1;
        for(int i = 0; i < 4; i++) {
            int reward = board(b).slide(i);
            if (reward > big) {
                big = reward;
                op = i;
            }
        }
        return op;
    }

    virtual action take_action(const board& before, action last) {
        int op = search(before);
        if (op != -1) return action::slide(op);
        return action();
    }

private:
    std::array<int, 4> opcode;
};
