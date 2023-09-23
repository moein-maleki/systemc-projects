#include "../bin/wire.hpp"

wire::wire(string n):
    name(n),
    level(NOT_SET),
    value(LOGIC_X),
    ctrl1(NOT_SET),
    ctrl0(NOT_SET),
    shell1(false),
    shell0(false) {}

bool wire::is_in(vector<wire*> wv) {
    for(wire* w: wv)
        if(this == w)
            return true;
    return false;
}