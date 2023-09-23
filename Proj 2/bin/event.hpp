#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include "./libs.hpp"

enum change{TO_0, TO_1, TO_X};

using namespace std;

class wire;
class event{
    public:
        event(wire*, change, int);
        wire* wire_e;
        change change_e;
        int time_e; 
};

change convert_to_change(char c);
string c2s(change c);

#endif