#include "tdf2De.h"

void tdf2De::set_attributes() {
    set_timestep(sc_time(GLOBAL_TIMESTEP_NS, SC_NS));
}

void tdf2De::processing() {
    out = in.read().to_int();
}