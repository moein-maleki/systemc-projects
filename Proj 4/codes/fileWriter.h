#ifndef FILEWRITER_HH
#define FILEWRITER_HH

#include <systemc.h>
#include "interfaceClasses.h"
#include "macros.h"
#include "utilities.h"

SC_MODULE(fileWriter) {
	sc_in <sc_logic> clk;
	sc_port <get_burst_if> fileWriter_in;

	sc_lv<8>* processedImage;
	sc_lv<8>* newSegment;

	sc_event newSegmentArrived;
	sc_event segmentSettingDone;
	sc_event writeProcessedImage;

	int currentSegmentUnder;

	SC_CTOR(fileWriter)
	{
		processedImage = new sc_lv<8>[IMAGE_SIZE];
		newSegment = new sc_lv<8>[SEGMENT_SIZE];
		currentSegmentUnder = 0;

		SC_THREAD(initialzeMemoryArrays);
		SC_THREAD(receiveSegments);
		SC_THREAD(setInNewSegment);
		SC_THREAD(writeResultsIntoFile);
		
	};

	void initialzeMemoryArrays();
	void receiveSegments();
	void setInNewSegment();
	void writeResultsIntoFile();
};

#endif