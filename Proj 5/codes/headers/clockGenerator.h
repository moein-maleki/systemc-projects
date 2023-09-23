#ifndef _CLOCK_GEN_H_
#define _CLOCK_GEN_H_

#include <systemc.h>
#include "general.h"

SC_MODULE(clockGenerator) {
    sc_out<sc_logic> out;

    SC_CTOR(clockGenerator) :
        out("out") {
        
        SC_THREAD(clocking);
    }

    void clocking();
};

#endif