#include "fileReader.h"

void fileReader::readFiles()
{
	cout << "readFiles active" << endl;
	channelsData[RED] = readFile(RED_CHANNEL_FILE_NAME);
	channelsData[GREEN] = readFile(GREEN_CHANNEL_FILE_NAME);
	channelsData[BLUE] = readFile(BLUE_CHANNEL_FILE_NAME);

	cout << "reading done" << endl;
	readingDone.notify(SC_ZERO_TIME);
}

void fileReader::sendDataToGrayscale()
{
	cout << "sendDataToGrayscale active" << endl;
	wait(readingDone);

	cout << "sending channelsData to grayScaler" << endl;
	for (int px = 0; px < IMAGE_SIZE; px++) {
		sc_lv<8> pixel[3] = {
			channelsData[RED][px],
			channelsData[GREEN][px],
			channelsData[BLUE][px]
		};
		//cout << "fileReader: px " << px;
		fileReader_out->send_pixel(pixel);
	}
}