#include "channelClasses.h"

pixel_channel::pixel_channel(sc_in <sc_logic>* c): clk(c) {};

void pixel_channel::send_pixel(sc_lv<8>* pixel) {
	channelBusy.lock();
	dataReady.notify(SC_ZERO_TIME);
	wait(receiverReady);
	for (int color = 0; color < MAX_NO_COLORS; color++) {
		wait(clk->posedge_event());
		pixelChanneldata = pixel[color];
		//cout << pixelChanneldata.to_uint() << " ";
	}
}

void pixel_channel::get_pixel(sc_lv<8>* pixel) {
	wait(dataReady);
	receiverReady.notify(SC_ZERO_TIME);
	for (int color = 0; color < MAX_NO_COLORS; color++) {
		wait(clk->posedge_event());
		pixel[color] = pixelChanneldata;
	}
	/*cout << "\t got: pixel: " <<
		pixel[RED].to_uint() << " " <<
		pixel[GREEN].to_uint() << " " <<
		pixel[BLUE].to_uint() << endl; */
	channelBusy.unlock();
}

burst_channel_reciever_based::burst_channel_reciever_based(sc_in <sc_logic>* c) {
	clk = c;
	requestedSegment = false;
}

bool burst_channel_reciever_based::send_burst(sc_lv<8>* segment, int burstSize) {
	if (requestedSegment == false) {
		return false;
	}
	segmentRequestReady.notify(SC_ZERO_TIME);
	wait(responceAcknowledged);
	cout << "REQUEST OBSERVED" << endl;
	for (int px = 0; px < burstSize; px++) {
		wait(clk->posedge_event());
		pixelData = segment[px];
		wait(SC_ZERO_TIME);
	}
	wait(clk->posedge_event());
	pixelData = 0;
	channelBusy.unlock();
	return true;
}

bool burst_channel_reciever_based::get_burst(sc_lv<8>* segment, int burstSize) {
	channelBusy.lock();
	cout << "REQUESTED A SEGMENT" << endl;
	requestedSegment = true;
	wait(segmentRequestReady);
	responceAcknowledged.notify(SC_ZERO_TIME);
	cout << "NOW RECEIVING A SEGMENT" << endl;
	for (int px = 0; px < burstSize; px++) {
		wait(clk->posedge_event());
		wait(SC_ZERO_TIME);
		segment[px] = pixelData;
	}
	requestedSegment = false;
	
	return true;
}

burst_channel_initiator_based::burst_channel_initiator_based(sc_in <sc_logic>* c) {
	clk = c;
}

bool burst_channel_initiator_based::send_burst(sc_lv<8>* segment, int burstSize) {
	channelBusy.lock();
	segmentReadyToBeSent.notify(SC_ZERO_TIME);
	wait(receiverReady);
	for (int px = 0; px < burstSize; px++) {
		wait(clk->posedge_event());
		pixelData = segment[px];
		wait(SC_ZERO_TIME);
	}
	return true;
}

bool burst_channel_initiator_based::get_burst(sc_lv<8>* segment, int burstSize) {
	wait(segmentReadyToBeSent);
	receiverReady.notify(SC_ZERO_TIME);
	for (int px = 0; px < burstSize; px++) {
		wait(clk->posedge_event());
		wait(SC_ZERO_TIME);
		segment[px] = pixelData;
	}
	channelBusy.unlock();
	return true;
}