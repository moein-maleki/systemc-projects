#include "grayScaler.h"

void grayScaler::getAndComputeGrayScale() {
	cout << "getAndComputeGrayScale active" << endl;
	for (int segmentNumber = 0; segmentNumber < NUM_OF_SEGMENTS; segmentNumber++) {
		for (int pxInSegment = 0; pxInSegment < SEGMENT_SIZE; pxInSegment++) {
			sc_lv<8> pixel[MAX_NO_COLORS];
			grayScaler_in->get_pixel(pixel);
			sc_uint<8> average = grayScale_of(pixel);
			grayScaledData[segmentNumber * SEGMENT_SIZE + pxInSegment] = (sc_lv<8>) average;
		}
		lastReadySegment++;
		newSegmentReady.notify(SC_ZERO_TIME);
		cout << "segment " << segmentNumber << " is ready." << endl;
	}
	cout << "------- grayScaler: FINISHED ALL SEGMENTS: --------" << endl;
	printSegment(grayScaledData, IMAGE_ROWS_PX);
	cout << "--------------------------------------" << endl;
	writeImageToFile(grayScaledData, GRAYSCALE_FILE_NAME);	
}

void grayScaler::initializeInternalArrays() {
	for (int px = 0; px < IMAGE_SIZE; px++) {
		grayScaledData[px] = 0;
	}
	for (int px = 0; px < SEGMENT_SIZE; px++) {
		newSegment[px] = 0;
	}
	wait(SC_ZERO_TIME);
}

void grayScaler::sendDataToEdgeDetector() {
	cout << "sendDataToEdgeDetector active" << endl;
	while (1) {
		wait(newSegmentReady);
		while (lastServedSegment != lastReadySegment) {
			//newSegment = selectSegment(grayScaledData, lastServedSegment + 1);
			bool aRequestWasObserved =
				grayScaler_out->send_burst(&grayScaledData[(lastServedSegment + 1)*SEGMENT_SIZE], SEGMENT_SIZE);
			if (aRequestWasObserved == false) {
				wait(SC_ZERO_TIME);
				continue;
			}
			lastServedSegment++;
			cout << "------- grayScaler: sent segment number: " << lastServedSegment << " --------" << endl;
			printSegment(newSegment, SEGMENT_ROWS_PX);
			cout << "--------------------------------------" << endl << endl;
			cout << "grayScaler served segment " << lastServedSegment << " to edgeDetector " << endl;
		}
	}
}


