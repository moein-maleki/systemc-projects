#include "processor.h"

void processor::bracketing() {
    std::vector<int> dataFrame(100);
    wait();
    while(1) {
        for(int iter = 0; iter < 100; iter++) {
            // starting the timer
            writeIO = SC_LOGIC_1;
            readIO = SC_LOGIC_0;
            addrBus = TIMER_BASE_ADDRESS;
            wait();

            // waiting for the 1ms interval to pass
            writeIO = SC_LOGIC_0;
            readIO = SC_LOGIC_1;
            wait();
            while(dataBus.read().to_uint() != 1) {
                wait();
            }
            
            //reading the sensor value
            writeIO = SC_LOGIC_0;
            readIO = SC_LOGIC_1;
            addrBus = SENSOR_BASE_ADDRESS;
            wait();
            dataFrame[iter] = dataBus.read().to_int();
        }
        double avg = std::accumulate(dataFrame.begin(), dataFrame.end(), 0.0) / dataFrame.size();
        std::cout << "average = " << avg << endl;
    }
}