#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include <systemc.h>
#include "general.h"
#include <iostream>
#include <vector>
#include <numeric>

SC_MODULE(processor) {
    sc_in <sc_logic> clk;

    sc_out <sc_logic> readIO;
    sc_out <sc_logic> writeIO;
    sc_out <sc_lv<16>> addrBus;

    sc_inout_rv <16> dataBus;

    SC_CTOR(processor) :
        clk("clk"),
        readIO("readIO"),
        writeIO("writeIO"),
        addrBus("addrBus"),
        dataBus("dataBus") {
        
        SC_THREAD(bracketing);
            sensitive << clk.pos();
        //dont_initialize();
    }

    void bracketing();
};

#endif