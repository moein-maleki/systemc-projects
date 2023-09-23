#ifndef __WIRE_HPP__
#define __WIRE_HPP__

#include "./libs.hpp"

#define NOT_SET -1

using namespace std;

enum logic3{LOGIC_0, LOGIC_1, LOGIC_X};

class wire {
    public:
        wire(string n);
        float c1() {return ctrl1; }
        float c0() {return ctrl0; }
        float assume_c1() {shell1 = true; return ctrl1; }
        float assume_c0() {shell0 = true; return ctrl0; }
        void clear_shells() {shell1 = false; shell0 = false; }
        bool already_assumed_1() {return shell1; }
        bool already_assumed_0() {return shell0; }
        int level;
        string name;
        logic3 value;
        bool is_in(vector<wire*> wv);
        void set_ctrl1(float f) {ctrl1 = f; ctrl0 = 1-f;};
    private:
        bool shell1;
        bool shell0;
        float ctrl1;
        float ctrl0;
};

#endif