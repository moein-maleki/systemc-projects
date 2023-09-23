#include "lowpassFilter.h"

lowpassFilter::lowpassFilter(sc_module_name) {
    inConverter = new sca_eln::sca_de::sca_vsource("converter_de2eln", 1.0);
    inConverter->inp(in);
    inConverter->p(inNode);
    inConverter->n(gndNode);
    inConverter->set_timestep(GLOBAL_TIMESTEP_NS, SC_NS);

    resUnit = new sca_eln::sca_r("lowpassFilter_res1", LOWPASS_FILTER_RES_1_VALUE);
    resUnit->n(inNode);
    resUnit->p(outNode);

    capUnit = new sca_eln::sca_c("lowpassFilter_cap1", LOWPASS_FILTER_CAP_1_VALUE);
    capUnit->n(outNode);
    capUnit->p(gndNode);

    outConverter = new sca_eln::sca_tdf::sca_vsink("converter_eln2tdf", 1.0);
    outConverter->outp(out);
    outConverter->p(outNode);
    outConverter->n(gndNode);
}