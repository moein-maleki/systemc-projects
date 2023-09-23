#ifndef _BUS_INTERFACE_H_
#define _BUS_INTERFACE_H_

#include <systemc.h>
#include "general.h"

SC_MODULE(busInterface) {
    sc_in <sc_logic> readIO;
    sc_in <sc_logic> writeIO;
    sc_in <sc_lv<16>> addrBus;

    sc_out <sc_logic> timerEnable;
    sc_out <sc_logic> sensorEnable;
    sc_out <sc_logic> startTimer;

    SC_CTOR(busInterface) :
        readIO("readIO"),
        writeIO("writeIO"),
        addrBus("addrBus"),
        timerEnable("timerEnable"),
        sensorEnable("sensorEnable"),
        startTimer("startTimer") {
        
        SC_THREAD(timerDecoder);
            sensitive << readIO << writeIO << addrBus;
        SC_THREAD(sensorDecoder);
            sensitive << readIO << writeIO << addrBus;
    }

    void timerDecoder();
    void sensorDecoder();
};

#endif