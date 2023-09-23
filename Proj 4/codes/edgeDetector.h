#ifndef EDGEDETECTOR_HH
#define EDGEDETECTOR_HH

#include <systemc.h>
#include "interfaceClasses.h"
#include "macros.h"
#include "utilities.h"

SC_MODULE(edgeDetector) {
	sc_in <sc_logic> clk;
	sc_port <get_burst_if> edgeDetector_in;
	sc_port <put_burst_if> edgeDetector_out;

	sc_lv<8>* segmentSlot;
	sc_lv<8>* newSegment;
	sc_lv<8>* lastSegmentsLastRow;

	sc_event reorganize;
	sc_event reorganizeDone;
	sc_event setNewSegmentIn;
	sc_event setNewSegmentInDone;
	sc_event applyKernelsToSegment;
	sc_event applyKernelsToSegmentDone;
	sc_event sendSegmentToFileWriter;
	sc_event sendSegmentToFileWriterDone;

	int currentSegmentUnder;
	int slotSize;
	int* Gx;
	int* Gy;

	SC_CTOR(edgeDetector)
	{
		segmentSlot = new sc_lv<8>[IMAGE_COLS_PX * (SEGMENT_ROWS_PX + 2)];
		newSegment = new sc_lv<8>[SEGMENT_SIZE];
		lastSegmentsLastRow = new sc_lv<8>[IMAGE_COLS_PX];

		Gx = new int[(SEGMENT_ROWS_PX) * (IMAGE_COLS_PX-2)];
		Gy = new int[(SEGMENT_ROWS_PX) * (IMAGE_COLS_PX-2)];

		currentSegmentUnder = 0;
		slotSize = IMAGE_COLS_PX * (SEGMENT_ROWS_PX + 2);

		SC_THREAD(initialzeSegmentSlot);
		SC_THREAD(getAndDetectEdge);
		SC_THREAD(reorganizeRows);
		SC_THREAD(setNewSegmentInSlot);
		SC_THREAD(processSegmentSlot);
		SC_THREAD(sendDataToFileWriter);
	};

	void initialzeSegmentSlot();
	void getAndDetectEdge();
	void reorganizeRows();
	void setNewSegmentInSlot();
	void processSegmentSlot();
	void sendDataToFileWriter();
};

#endif