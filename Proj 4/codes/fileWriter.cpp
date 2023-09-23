#include "fileWriter.h"

void fileWriter::initialzeMemoryArrays() {
	for (int px = 0; px < IMAGE_SIZE; px++) {
		processedImage[px] = 0;
	}
	for (int px = 0; px < SEGMENT_SIZE; px++) {
		newSegment[px] = 0;
	}
}


void fileWriter::setInNewSegment() {
	while (1) {
		wait(newSegmentArrived);
		int burstSize = (currentSegmentUnder == 0) ? ((SEGMENT_ROWS_PX - 1) * IMAGE_COLS_PX) :
			(currentSegmentUnder == NUM_OF_SEGMENTS) ? (IMAGE_COLS_PX) : (SEGMENT_SIZE);
		int writeOffset = (currentSegmentUnder == 0) ? (0) :
			((currentSegmentUnder - 1) * SEGMENT_SIZE + (SEGMENT_ROWS_PX - 1) * IMAGE_COLS_PX);
		for (int px = 0; px < burstSize; px++) {
			processedImage[writeOffset + px] = newSegment[px];
		}

		cout << "------- fileWriter: setInNewSegment result: " << currentSegmentUnder << " --------" << endl;
		printSegment(&processedImage[writeOffset], burstSize / IMAGE_COLS_PX);
		cout << "--------------------------------------" << endl;

		segmentSettingDone.notify(SC_ZERO_TIME);
	}
}

void fileWriter::receiveSegments() {
	while (1) {
		if (currentSegmentUnder == NUM_OF_SEGMENTS + 1) {
			cout << "################ currentSegmentUnder == NUM_OF_SEGMENTS + 1" << endl;
			break;
		}
		int burstSize = (currentSegmentUnder == 0) ? ((SEGMENT_ROWS_PX - 1) * IMAGE_COLS_PX) :
			(currentSegmentUnder == NUM_OF_SEGMENTS) ? (IMAGE_COLS_PX) : (SEGMENT_SIZE);
		cout << "################ currentSegmentUnder = " << currentSegmentUnder << ", burstSize = " << burstSize << endl;
		fileWriter_in->get_burst(newSegment, burstSize);
		cout << "################ recieved successfully = " << currentSegmentUnder << ", burstSize = " << burstSize << endl;

		cout << "------- fileWriter: got segment number: " << currentSegmentUnder << " --------" << endl;
		printSegment(newSegment, burstSize/IMAGE_COLS_PX);
		cout << "--------------------------------------" << endl;

		newSegmentArrived.notify(SC_ZERO_TIME);
		wait(segmentSettingDone);

		cout << "------- fileWriter: image after setting segment " << currentSegmentUnder << " --------" << endl;
		printSegment(processedImage, IMAGE_ROWS_PX);
		cout << "--------------------------------------" << endl;
		currentSegmentUnder++;
	}
	cout << "################ FILE WRITER RECEIVED ALL SEGMENTS " << endl;
	writeProcessedImage.notify(SC_ZERO_TIME);
}

void fileWriter::writeResultsIntoFile() {
	wait(writeProcessedImage);
	cout << "################ writeProcessedImage notified" << endl;
	writeImageToFile(processedImage, FINAL_IMAGE_FILE_NAME);

	cout << "------- fileWriter: final image result --------" << endl;
	printSegment(processedImage, IMAGE_ROWS_PX);
	cout << "--------------------------------------" << endl;

	cout << "image processing completed." << endl;
	//sc_core::sc_stop();
}