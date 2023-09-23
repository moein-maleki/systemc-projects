#ifndef __SQUARE_WAVE_HPP__
#define __SQUARE_WAVE_HPP__

#include <systemc.h>
#include <systemc-ams.h>
#include "general.h"

SC_MODULE(squareWave) {
	sc_out <double> out;
	
	SC_CTOR(squareWave) :
		out("out") {
		
		SC_THREAD(squareWaveGeneration);
	}

	void squareWaveGeneration();
};


#endif