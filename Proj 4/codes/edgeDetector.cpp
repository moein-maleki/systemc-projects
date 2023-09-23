#include "edgeDetector.h"

void edgeDetector::initialzeSegmentSlot() {
	for (int px = 0; px < slotSize; px++) {
		segmentSlot[px] = 0;
	}
	for (int px = 0; px < SEGMENT_ROWS_PX * (IMAGE_COLS_PX - 2); px++) {
		Gx[px] = 0;
		Gy[px] = 0;
	}
	for (int px = 0; px < IMAGE_COLS_PX; px++) {
		lastSegmentsLastRow[px] = 0;
	}
	wait(SC_ZERO_TIME);
}

void edgeDetector::reorganizeRows() {
	while (1) {
		wait(reorganize);
		wait(SC_ZERO_TIME);

		cout << "------- edgeDetector: segmentSlot before moving lastSegmentsLastRow " << currentSegmentUnder << "  --------" << endl;
		printSegment(segmentSlot, SEGMENT_ROWS_PX + 2);
		cout << "--------------------------------------" << endl;

		copyLogicVectors(segmentSlot, lastSegmentsLastRow, 0, IMAGE_COLS_PX);
		wait(SC_ZERO_TIME);

		cout << "------- edgeDetector: lastSegmentsLastRow before moving into position: " << currentSegmentUnder << "  --------" << endl;
		printSegment(lastSegmentsLastRow, 1);
		cout << "--------------------------------------" << endl;

		wait(SC_ZERO_TIME);
		for (int px = 0; px < IMAGE_COLS_PX; px++)
			segmentSlot[IMAGE_COLS_PX + px] = segmentSlot[(SEGMENT_ROWS_PX + 1) * IMAGE_COLS_PX + px];

		cout << "------- edgeDetector: segmentSlot status after moving the last row of the last segment" << currentSegmentUnder << "  --------" << endl;
		printSegment(segmentSlot, SEGMENT_ROWS_PX + 2);
		cout << "--------------------------------------" << endl;

		reorganizeDone.notify(SC_ZERO_TIME);
	}
}

void edgeDetector::setNewSegmentInSlot() {
	while (1) {
		wait(setNewSegmentIn);
		for (int px = 0; px < SEGMENT_SIZE; px++) {
			segmentSlot[px + 2 * IMAGE_COLS_PX] = newSegment[px];
		}
		setNewSegmentInDone.notify(SC_ZERO_TIME);

		cout << "------- edgeDetector: segmentSlot status after set new segment in: " << currentSegmentUnder << "   --------" << endl;
		printSegment(segmentSlot, SEGMENT_ROWS_PX + 2);
		cout << "--------------------------------------" << endl;
	}
}

void edgeDetector::processSegmentSlot() {
	while (1) {
		wait(applyKernelsToSegment);
		copyLogicVectors(lastSegmentsLastRow, segmentSlot, SEGMENT_SIZE, IMAGE_COLS_PX);
		
		for (int y = 1; y < SEGMENT_ROWS_PX + 1; y++) {
			for (int x = 1; x < IMAGE_COLS_PX - 1; x++) {
				Gx[(y-1) * IMAGE_COLS_PX + x-1] = calculateGx(x, y, segmentSlot);
				Gy[(y-1) * IMAGE_COLS_PX + x-1] = calculateGy(x, y, segmentSlot);
			}
		}

		wait(SC_ZERO_TIME);

		cout << "------- edgeDetector: lastSegmentsLastRow set to: " << currentSegmentUnder << "  --------" << endl;
		printSegment(lastSegmentsLastRow, 1);
		cout << "--------------------------------------" << endl;

		applyKernelArrays(segmentSlot, Gx, Gy, currentSegmentUnder);

		cout << "------- edgeDetector: segmentSlot status after processed: " << currentSegmentUnder << "  --------" << endl;
		printSegment(segmentSlot, SEGMENT_ROWS_PX + 2);
		cout << "--------------------------------------" << endl;
		cout << "edge detector: processed segment number: " << currentSegmentUnder << endl;
		
		applyKernelsToSegmentDone.notify(SC_ZERO_TIME);

	}
}

void edgeDetector::getAndDetectEdge() {
	while (1) {
		if (currentSegmentUnder == NUM_OF_SEGMENTS)
			break;
		reorganize.notify(SC_ZERO_TIME);
		wait(reorganizeDone);

		if (currentSegmentUnder == NUM_OF_SEGMENTS - 1)
			cout << "poof" << endl;
		cout << "################## edgeDetector: getting new segment: currentSegmentUnder = " << currentSegmentUnder << endl;
		edgeDetector_in->get_burst(newSegment, SEGMENT_SIZE);

		cout << "------- edgeDetector: got segment number: " << currentSegmentUnder << " from grayScaler" << endl;
		printSegment(newSegment, SEGMENT_ROWS_PX);
		cout << "--------------------------------------" << endl;

		setNewSegmentIn.notify(SC_ZERO_TIME);
		wait(setNewSegmentInDone);

		applyKernelsToSegment.notify(SC_ZERO_TIME);
		wait(applyKernelsToSegmentDone);

		sendSegmentToFileWriter.notify(SC_ZERO_TIME);
		wait(sendSegmentToFileWriterDone);
	}
	cout << "------- edgeDetector: GOT ALL SEGMENTS AND SENT ALL " << endl;
}

void edgeDetector::sendDataToFileWriter() {
	while (currentSegmentUnder != NUM_OF_SEGMENTS) {
		cout << "################## edgeDetector: currentSegmentUnder = " << currentSegmentUnder << endl;
		wait(sendSegmentToFileWriter);
		cout << "edgeDetector: now sending segment " << currentSegmentUnder << " to fileWriter" << endl;
		int segmentOffset = (currentSegmentUnder == 0) ? (2 * IMAGE_COLS_PX) : (IMAGE_COLS_PX);
		int burstSize = (currentSegmentUnder == 0) ? ((SEGMENT_ROWS_PX - 1) * IMAGE_COLS_PX) : (SEGMENT_SIZE);

		edgeDetector_out->send_burst(&segmentSlot[segmentOffset], burstSize);
		cout << "################## sent the last row successfully. --------" << endl;

		cout << "------- edgeDetector: sent segment number: " << currentSegmentUnder << " --------" << endl;
		printSegment(&segmentSlot[segmentOffset], burstSize/IMAGE_COLS_PX);
		cout << "--------------------------------------" << endl;

		currentSegmentUnder++;
		sendSegmentToFileWriterDone.notify();
	}
	wait(clk->posedge_event());
	cout << "################## out of the blue. --------" << endl;
	edgeDetector_out->send_burst(&segmentSlot[(SEGMENT_ROWS_PX + 1) * IMAGE_COLS_PX], IMAGE_COLS_PX);
	cout << "################## sent the last row successfully. --------" << endl;
	printSegment(&segmentSlot[(SEGMENT_ROWS_PX + 1) * IMAGE_COLS_PX], 1);
	cout << "--------------------------------------" << endl;
}