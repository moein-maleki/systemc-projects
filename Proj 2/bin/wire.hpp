#ifndef __WIRE_HPP__
#define __WIRE_HPP__

#include "./libs.hpp"
#include "./event.hpp"

#define NOT_SET -1

using namespace std;

enum logic3{LOGIC_0, LOGIC_1, LOGIC_X};

class wire {
    public:
        wire(string n);
        event* add_event(change event_change, int event_time);
        bool lock_obs();
        float get_ctrl1() {return ctrl1; }
        float get_ctrl0() {return ctrl0; }
        float assume_c1() {assume_1 = true; return ctrl1; }
        float assume_c0() {assume_0 = true; return ctrl0; }
        void clear_shells() {assume_1 = false; assume_0 = false; }
        bool already_assumed_1() {return assume_1; }
        bool already_assumed_0() {return assume_0; }
        bool is_in(vector<wire*> wv);
        void set_ctrl1(float f) { ctrl1 = f; ctrl0 = 1.0-f;};
        void add_obs(float f);
        float read_obs() {return obs; }
        logic3 get_value() {return value; }
        void set_value(logic3 v);
        change last_event_before(int t);

        int level;
        string name;
        vector<event*> event_vector;
    private: 
        logic3 value;
        vector<float> obses_set;
        float obs;
        bool assume_1;
        bool assume_0;
        float ctrl1;
        float ctrl0;
};


string l2s(logic3 c);

#endif