#include "../bin/event.hpp"

event::event(wire* w, change c, int i)
: wire_e(w), change_e(c), time_e(i) {}

change convert_to_change(char c) {
    if(c == '0') {
        return TO_0;
    } else if(c == '1') {
        return TO_1;
    } else {
        return TO_X;
    }
}

string c2s(change c) {
    switch (c)
    {
    case TO_X:
        return "x";
    case TO_1:
        return "1";
    case TO_0:
        return "0";
    default:
        return "F";
    }
}
