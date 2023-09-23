#include "main.h"

int sc_main(int argc, char* argv[]) {
    sc_set_time_resolution(1.0, SC_NS);
	sca_util::sca_trace_file* trace = sca_util::sca_create_vcd_trace_file("traceFile");
	
	// signal instantiation
	sc_signal <double> 				squareWaveSig;
	sc_signal <sc_logic> 			clk;
	sc_signal <sc_logic> 			readIO;
	sc_signal <sc_logic> 			writeIO;
	sc_signal <sc_logic> 			sensorEnable;
	sc_signal <sc_logic> 			timerEnable;
	sc_signal <sc_logic> 			startTimer;
	sc_signal <sc_lv<16>> 			sensorOut;
	sc_signal <sc_lv<16>> 			addrBus;
	sc_signal <sc_lv<16>>			timeOut;
	sc_signal <sc_lv<16>>			registerOut;
	sc_signal_rv <16>				dataBus;

	// component instantiation
	squareWave 						squareWaveUnit(					"squareWave_inst");
	sensorFrontend 					sensorFrontendUnit(				"sensorFrontend_inst");
	clockGenerator 					clockGeneratorUnit(				"clockGenerator_inst");
	regizter 						registerUnit(					"register_inst");
	timer 							timerUnit(						"timer_inst");
	busInterface 					busInterfaceUnit(				"busInterface_inst");
	processor 						processorUnit(					"processor_inst");

	//creating connections
	squareWaveUnit.out(				squareWaveSig);
	clockGeneratorUnit.out(			clk);

	sensorFrontendUnit.in(			squareWaveSig);
	sensorFrontendUnit.out(			sensorOut);
		
	registerUnit.clk(				clk);
	registerUnit.outEnable(			sensorEnable);
	registerUnit.in(				sensorOut);
	registerUnit.out(				dataBus);

	timerUnit.clk(					clk);
    timerUnit.startTimer(			startTimer);
    timerUnit.outEnable(			timerEnable);
    timerUnit.timeOut(				dataBus);

    busInterfaceUnit.readIO(		readIO);
    busInterfaceUnit.writeIO(		writeIO);
    busInterfaceUnit.addrBus(		addrBus);
    busInterfaceUnit.timerEnable(	timerEnable);
    busInterfaceUnit.sensorEnable(	sensorEnable);
    busInterfaceUnit.startTimer(	startTimer);

    processorUnit.clk(				clk);
    processorUnit.dataBus(			dataBus);
    processorUnit.readIO(			readIO);
    processorUnit.writeIO(			writeIO);
    processorUnit.addrBus(			addrBus);

	// tracing signals
	sca_util::sca_trace(trace, 		squareWaveUnit.out, 				"squareWave");
	sca_util::sca_trace(trace, 		clockGeneratorUnit.out, 			"clk");
	
	sca_util::sca_trace(trace, 		sensorFrontendUnit.sineSignal, 		"lowPassFilterOut");
	sca_util::sca_trace(trace, 		sensorFrontendUnit.adcSineSignal, 	"adcOut");
	sca_util::sca_trace(trace, 		sensorFrontendUnit.out, 			"tdf2DeOut");

	// sca_util::sca_trace(trace, 		registerUnit.out, 					"registerOut");
	sca_util::sca_trace(trace, 		registerUnit.outEnable, 			"sensorEnable");
	
	// sca_util::sca_trace(trace, 		timerUnit.timeOut, 					"timeOut");
	sca_util::sca_trace(trace, 		timerUnit.outEnable, 				"timerEnable");
	sca_util::sca_trace(trace, 		timerUnit.startTimer, 				"startTimer");
	sca_util::sca_trace(trace, 		timerUnit.microsPassed, 			"microsPassed");
	sca_util::sca_trace(trace, 		timerUnit.timerActive, 				"timerActive"); 

	sca_util::sca_trace(trace, 		busInterfaceUnit.timerEnable, 		"timerEnable");
	sca_util::sca_trace(trace, 		busInterfaceUnit.sensorEnable, 		"sensorEnable");
	sca_util::sca_trace(trace, 		busInterfaceUnit.startTimer, 		"startTimer");

	sca_util::sca_trace(trace, 		processorUnit.readIO, 				"readIO");
	sca_util::sca_trace(trace, 		processorUnit.writeIO, 				"writeIO");
	sca_util::sca_trace(trace, 		processorUnit.addrBus, 				"addrBus");
	
	// sca_util::sca_trace(trace, 		dataBus, 							"dataBus");

	sc_start(1200, SC_MS);

	sca_util::sca_close_vcd_trace_file(trace);
    return 0;
}