#ifndef _SENSORFRONTEND_H_
#define _SENSORFRONTEND_H_

#include <systemc.h>
#include <systemc-ams.h>
#include "general.h"
#include "lowpassFilter.h"
#include "adc.h"
#include "tdf2De.h"

SC_MODULE(sensorFrontend) {
    sc_core::sc_in <double> in;

    sc_core::sc_out <sc_dt::sc_lv <ADC_OUTPUT_BITS>> out;

    lowpassFilter* lowpassFilterUnit;
    adc* adcUnit;
    tdf2De* tdf2DeUnit;

    SC_HAS_PROCESS(sc_module_name);
    sensorFrontend(sc_module_name);

    sca_tdf::sca_signal<double> sineSignal;
    sca_tdf::sca_signal<sc_dt::sc_int<ADC_OUTPUT_BITS>> adcSineSignal;
};

#endif