#include "busInterface.h"

void busInterface::timerDecoder() {
    while(1) {
        timerEnable = SC_LOGIC_0;
        startTimer = SC_LOGIC_0;
        if((addrBus.read().to_uint() & BUS_ADDRESS_IO_MASK) == (TIMER_BASE_ADDRESS & BUS_ADDRESS_IO_MASK)) {
            if(readIO == SC_LOGIC_1) {
                timerEnable = SC_LOGIC_1;
            }
            else if(writeIO == SC_LOGIC_1) {
                startTimer = SC_LOGIC_1;
            }
        }
        wait();
    }
}

void busInterface::sensorDecoder() {
    while(1) {
        sensorEnable = SC_LOGIC_0;
        if((addrBus.read().to_uint() & BUS_ADDRESS_IO_MASK) == (SENSOR_BASE_ADDRESS & BUS_ADDRESS_IO_MASK)) {
            if(readIO == SC_LOGIC_1) {
                sensorEnable = SC_LOGIC_1;
            }
        }
        wait();
    }
}