#include "timer.h"

void timer::activating() {
    timerActive = SC_LOGIC_0;
    while(1) {
        if(startTimer == SC_LOGIC_0 && microsPassed.read().to_uint() == TIMER_TIMEOUT_INTERVAL_US) {
            timerActive = SC_LOGIC_0;
        }
        else if(startTimer == SC_LOGIC_1) {
            while(1) {
                wait();
                if(startTimer == SC_LOGIC_0) {
                    timerActive = SC_LOGIC_1;
                    break;
                }
            }
        }  
        wait();
    }
}
void timer::counting() {
    microsPassed = 0;
    while(1) {
        if(timerActive == SC_LOGIC_1) {
            if(microsPassed.read().to_uint() == TIMER_TIMEOUT_INTERVAL_US) {
                // hold the microsPassed as it is, to keep the timeOut on
            }
            else {
                microsPassed = (sc_lv<16>) (microsPassed.read().to_uint() + 1);
            }
        }
        if(startTimer == SC_LOGIC_1) {
            microsPassed = (sc_lv<16>) 0;
        }
        wait();
    }
}

void timer::tristate() {
    while(1) {
        if(outEnable == SC_LOGIC_1){
            if((timerActive == SC_LOGIC_1) && (microsPassed.read().to_uint() == TIMER_TIMEOUT_INTERVAL_US)) {
                timeOut.write(TIMER_TIMEOUT_SET_VALUE); // = (sc_lv<16>) 1
            }
            else {
                timeOut.write(TIMER_TIMEOUT_CLEAR_VALUE); // = (sc_lv<16>) 0
            }
        }
        else {
            timeOut.write(TIMER_TIMEOUT_Z_VALUE);
        }
        wait();
    }
}
