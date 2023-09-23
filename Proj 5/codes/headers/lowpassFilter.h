#ifndef __LOWPASS_FILTER_HPP__
#define __LOWPASS_FILTER_HPP__

#include <systemc.h>
#include <systemc-ams.h>
#include "general.h"

SC_MODULE(lowpassFilter) {
	sc_in<double> in;
	
	sca_tdf::sca_out<double> out;
    
	sca_eln::sca_r* resUnit;
	sca_eln::sca_c* capUnit;

	sca_eln::sca_de::sca_vsource* inConverter;
	sca_eln::sca_tdf::sca_vsink* outConverter;

    SC_HAS_PROCESS(sc_module_name);
    lowpassFilter(sc_module_name);
		
	private:
		sca_eln::sca_node_ref gndNode;
		sca_eln::sca_node inNode;
		sca_eln::sca_node outNode;
};

#endif