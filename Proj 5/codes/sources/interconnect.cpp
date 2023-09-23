#include "interconnect.h"

void interconnect::handle() {
    while(1) {
        if(timerEnable == SC_LOGIC_1 && sensorEnable == SC_LOGIC_1) {
            sharedOut = "XXXXXXXXXXXXXXXX"; 
        }
        else if(timerEnable == SC_LOGIC_1) {
            sharedOut = timeOut;    
        }
        else if(sensorEnable == SC_LOGIC_1) {
            sharedOut = registerOut;    
        }
        else {
            sharedOut = "ZZZZZZZZZZZZZZZZ";    
        }
        wait();
    }
}