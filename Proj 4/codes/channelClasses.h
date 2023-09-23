#ifndef CHANNELS_HH
#define CHANNELS_HH

#include <systemc.h>
#include "interfaceClasses.h"
#include "macros.h"

class pixel_channel : public put_pixel_if, public get_pixel_if {
	public:
		pixel_channel(sc_in <sc_logic>*);
		void send_pixel(sc_lv<8>* pixel);
		void get_pixel(sc_lv<8>* pixel);
	private:
		sc_event dataReady;
		sc_event receiverReady;
		sc_in <sc_logic>* clk;
		sc_lv<8> pixelChanneldata;
		sc_mutex channelBusy;
};

class burst_channel_reciever_based : public put_burst_if, public get_burst_if {
public:
	burst_channel_reciever_based(sc_in <sc_logic>*);
	bool send_burst(sc_lv<8>* segment, int burstSize);
	bool get_burst(sc_lv<8>* segment, int burstSize);
private:
	sc_in <sc_logic>* clk;
	bool requestedSegment;
	sc_lv<8> pixelData;

	sc_event segmentRequestReady;
	sc_event responceAcknowledged;

	sc_mutex channelBusy;
};

class burst_channel_initiator_based : public put_burst_if, public get_burst_if {
public:
	burst_channel_initiator_based(sc_in <sc_logic>*);
	bool send_burst(sc_lv<8>* segment, int burstSize);
	bool get_burst(sc_lv<8>* segment, int burstSize);
private:
	sc_in <sc_logic>* clk;
	sc_lv<8> pixelData;

	sc_event segmentReadyToBeSent;
	sc_event receiverReady;

	sc_mutex channelBusy;
};

#endif