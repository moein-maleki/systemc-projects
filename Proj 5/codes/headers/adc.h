#ifndef _ADC_H_
#define _ADC_H_

#include <systemc.h>
#include <systemc-ams.h>
#include <math.h>
#include "general.h"

SCA_TDF_MODULE(adc) {
    sca_tdf::sca_in<double> in;
    
    sca_tdf::sca_out<sc_dt::sc_int<ADC_OUTPUT_BITS>> out;

    adc(sc_core::sc_module_name) {}

    void set_attributes();
    void processing();
};

#endif