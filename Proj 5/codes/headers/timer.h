#ifndef _TIMER_H_
#define _TIMER_H_

#include <systemc.h>
#include "general.h"

SC_MODULE(timer) {
    sc_in <sc_logic> clk;
    sc_in <sc_logic> startTimer;
    sc_in <sc_logic> outEnable;

    sc_out_rv <16> timeOut;

    sc_signal <sc_lv<16>> microsPassed;
    sc_signal <sc_logic> timerActive;

    SC_CTOR(timer) {
        SC_THREAD(activating);
            sensitive << clk.pos();
        SC_THREAD(counting);
            sensitive << clk.pos();
        SC_THREAD(tristate);
            sensitive << outEnable << microsPassed;
    }

    void activating();
    void counting();
    void tristate();
};

#endif