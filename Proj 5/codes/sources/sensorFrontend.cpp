#include "sensorFrontend.h"

sensorFrontend::sensorFrontend(sc_module_name) {
    lowpassFilterUnit = new lowpassFilter("lowpassFilter_inst");
    adcUnit = new adc("adc_inst");
    tdf2DeUnit = new tdf2De("tdf2De_inst");

    lowpassFilterUnit->in(in);
    lowpassFilterUnit->out(sineSignal);
    
    adcUnit->in(sineSignal);
    adcUnit->out(adcSineSignal);

    tdf2DeUnit->in(adcSineSignal);
    tdf2DeUnit->out(out);
}