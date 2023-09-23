#include "../bin/wire.hpp"

wire::wire(string n):
    name(n),
    level(NOT_SET),
    value(LOGIC_X),
    ctrl1(NOT_SET),
    ctrl0(NOT_SET),
    assume_1(false),
    obs(NOT_SET),
    assume_0(false) {}

void wire::set_value(logic3 v) {
    value = v;
}

bool wire::is_in(vector<wire*> wv) {
    for(wire* w: wv)
        if(this == w)
            return true;
    return false;
}

event* wire::add_event(change event_change, int event_time) {
    if(event_change == last_event_before(event_time)) {
        return NULL;
    }
    event* new_event = new event(this, event_change, event_time);
    event_vector.push_back(new_event);
    
    cout << SIM_ACK << "event created:"
        << " wire=" << this->name
        << " old=" << l2s(this->value) 
        << " new=" << c2s(event_change) 
        << " time=" << event_time << endl;
    return new_event;
};

void wire::add_obs(float f) {
    obses_set.push_back(f);
}

bool wire::lock_obs() {
    if(obses_set.size() == 0) {
        return false;
    } else if(obs != NOT_SET){
        return true;
    } else {
        float sum = 0.0;
        for(float f: obses_set) {
            sum += f;
        }
        obs = sum / obses_set.size();
        return true;
    }
}

change wire::last_event_before(int t) {
    change lc = TO_X;
    for(event* e: event_vector) {
        if(e->time_e <= t) {
            lc = e->change_e;
        }
    }
    return lc;
}

string l2s(logic3 c) {
    if(c == LOGIC_1) {
        return "1";
    } else if(c == LOGIC_0) {
        return "0";
    }
    return "x";
}