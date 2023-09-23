#ifndef _REGISTER_H_
#define _REGISTER_H_

#include <systemc.h>
#include "general.h"

SC_MODULE(regizter) {
    sc_in <sc_logic> clk;
    sc_in <sc_logic> outEnable;
    sc_in <sc_lv<16>> in;

    sc_out_rv <16> out;

    sc_signal <sc_lv<16>> temp;

    SC_CTOR(regizter) :
        clk("clk"),
        outEnable("outEnable"),
        in("in"),
        out("out") {
            
        SC_THREAD(registering);
            sensitive << clk.pos();
        SC_THREAD(tristate);
            sensitive << outEnable << temp;
    }
    
    void registering();
    void tristate();
};

#endif

