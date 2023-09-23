#ifndef _INTERCONNECT_H_
#define _INTERCONNECT_H_

#include <systemc.h>

SC_MODULE(interconnect) {
    sc_in <sc_logic> timerEnable;
    sc_in <sc_logic> sensorEnable;
    sc_in <sc_lv<16>> timeOut;
    sc_in <sc_lv<16>> registerOut;
    
    sc_out <sc_lv<16>> sharedOut;

    SC_CTOR(interconnect) {
        SC_THREAD(handle);
            sensitive << timerEnable << sensorEnable << timeOut << registerOut;
    }

    void handle();
};

#endif

