#pragma once

#include <algorithm>
#include <limits>
#include <iostream>
#include <iomanip>

#include "state.h"

struct Result {
    int min, max, n;
    float avg;
    bool illegal;

    Result () : min(std::numeric_limits<int>::max()), max(std::numeric_limits<int>::lowest()), n(0), avg(0.0), illegal(false) {}
    Result (int v) : min(v), max(v), n(0), avg(v), illegal(false) {}
    Result (bool illegal) : illegal(illegal) {}

    void improve (const Type& type, const Result& result) {
        if (type == AFTERSTATE) {
            min = std::min(min, result.min);
            max = std::max(max, result.max);
            avg = (avg * n + result.avg) / (n + 1);
            n++;
        } else {
            if (result.avg > avg) *this = result;
        }
    }

    friend ostream& operator << (ostream& out, Result&& r) {
        if (r.illegal) out << -1;
        else out << std::fixed << std::setprecision(7) << r.min << " " << r.avg << " " << r.max;
        return out;
    }
};

Result ILLEGAL(true);
