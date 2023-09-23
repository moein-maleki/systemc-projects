#ifndef _PULSE_H_
#define _PULSE_H_

#include <systemc.h>
#include "general.h"

SC_MODULE(pulseGenerator) {
    double frequency_MHz;
    double pulseWidth_US;

    sc_out<sc_logic> out;
    
    SC_CTOR(pulseGenerator) :
        out("out") {
        
        SC_THREAD(pulsating);
    }

    void pulsating();
};

#endif
