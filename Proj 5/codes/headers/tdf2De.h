#ifndef _TDF2DE_H_
#define _TDF2DE_H_

#include <systemc.h>
#include <systemc-ams.h>
#include "general.h"

SCA_TDF_MODULE(tdf2De) {
    sca_tdf::sca_in<sc_dt::sc_int<ADC_OUTPUT_BITS>> in;
    
    sca_tdf::sca_de::sca_out<sc_lv<ADC_OUTPUT_BITS>> out;

    tdf2De(sc_module_name) {}

    void set_attributes();
    void processing();
};

#endif