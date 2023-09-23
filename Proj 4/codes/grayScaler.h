#ifndef GRAYSCALER_HH
#define GRAYSCALER_HH

#include <systemc.h>
#include "interfaceClasses.h"
#include "macros.h"
#include "utilities.h"

SC_MODULE(grayScaler) {
	sc_in <sc_logic> clk;
	sc_port <get_pixel_if> grayScaler_in;
	sc_port <put_burst_if> grayScaler_out;

	sc_lv<8>* grayScaledData;
	sc_lv<8>* newSegment;
	sc_event newSegmentReady;
	int lastReadySegment;
	int lastServedSegment;

	SC_CTOR(grayScaler)
	{
		grayScaledData = new sc_lv<8>[IMAGE_SIZE];
		newSegment = new sc_lv<8>[SEGMENT_SIZE];

		lastReadySegment = -1;
		lastServedSegment = -1;

		SC_THREAD(initializeInternalArrays);
		SC_THREAD(getAndComputeGrayScale);
		SC_THREAD(sendDataToEdgeDetector);
	};

	void initializeInternalArrays();
	void getAndComputeGrayScale();
	void sendDataToEdgeDetector();
};

#endif