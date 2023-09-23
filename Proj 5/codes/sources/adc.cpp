#include "adc.h"

void adc::set_attributes() {
    set_timestep(sc_time(GLOBAL_TIMESTEP_NS, SC_NS));
}

void adc::processing() {
    int threshold = (int)pow(2, ADC_OUTPUT_BITS - 1) - 1;

    if(in.read() > threshold)
        out.write(ADC_MAXIMUM_VALUE);
    else if(in.read() < -1 * threshold)
        out.write(-ADC_MAXIMUM_VALUE);
    else {
        out.write(lround(in.read() * (threshold/ADC_MAXIMUM_VALUE)));
    }
}