#ifndef FILE_READER_HH
#define FILE_READER_HH

#include <systemc.h>
#include "interfaceClasses.h"
#include "utilities.h"
#include "macros.h"

SC_MODULE(fileReader) {
	sc_in<sc_logic> clk;
	sc_port <put_pixel_if> fileReader_out;

	sc_lv<8> **channelsData;
	sc_event readingDone;

	SC_CTOR(fileReader)
	{
		channelsData = new sc_lv<8>*[MAX_NO_COLORS];

		SC_THREAD(readFiles);
		SC_THREAD(sendDataToGrayscale);
	};

	void readFiles();
	void sendDataToGrayscale();
};

#endif	