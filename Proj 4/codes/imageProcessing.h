#ifndef IMAGEPROCESSING_HH
#define IMAGEPROCESSING_HH

#include <systemc.h>
#include "channelClasses.h"
#include "fileReader.h"
#include "grayScaler.h"
#include "edgeDetector.h"
#include "fileWriter.h"

SC_MODULE(imageProcessing) {
	sc_in <sc_logic> clk;

	fileReader* fileReader_UUT;
	grayScaler* grayScaler_UUT;
	edgeDetector* edgeDetector_UUT;
	pixel_channel* pixel_channel_UUT;
	fileWriter* fileWriter_UUT;
	burst_channel_reciever_based * burst_channel_reciever_based_UUT;
	burst_channel_initiator_based * burst_channel_initiator_based_UUT;

	SC_CTOR(imageProcessing)
	{
		pixel_channel_UUT = new pixel_channel(&clk);
		burst_channel_reciever_based_UUT = new burst_channel_reciever_based(&clk);
		burst_channel_initiator_based_UUT = new burst_channel_initiator_based(&clk);

		fileReader_UUT = new fileReader("fileReader_UUT_testbench");
		fileReader_UUT->clk(clk);
		fileReader_UUT->fileReader_out(*pixel_channel_UUT);
		
		grayScaler_UUT = new grayScaler("grayScaler_UUT_testbench");
		grayScaler_UUT->clk(clk);
		grayScaler_UUT->grayScaler_in(*pixel_channel_UUT);
		grayScaler_UUT->grayScaler_out(*burst_channel_reciever_based_UUT);
		
		edgeDetector_UUT = new edgeDetector("edgeDetector_UUT_testbench");
		edgeDetector_UUT->clk(clk);
		edgeDetector_UUT->edgeDetector_in(*burst_channel_reciever_based_UUT);
		edgeDetector_UUT->edgeDetector_out(*burst_channel_initiator_based_UUT);

		fileWriter_UUT = new fileWriter("fileWriter_UUT_testbench");
		fileWriter_UUT->clk(clk);
		fileWriter_UUT->fileWriter_in(*burst_channel_initiator_based_UUT);
	};
};

#endif