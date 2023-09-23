#include "pulseGenerator.h"

void pulseGenerator::pulsating() {
    double zperiod_US = (1.0/frequency_MHz) - pulseWidth_US;
    while(1) {
        out.write(SC_LOGIC_1);
        wait(pulseWidth_US, SC_US);
        out.write(SC_LOGIC_0);
        wait(zperiod_US, SC_US);
    }
}