#include "utilities.h"

int grayScale_of(sc_lv<8>* pixel) {
	/*int temp = (
		pixel[RED].to_uint() +
		pixel[GREEN].to_uint() +
		pixel[BLUE].to_uint()
		) / MAX_NO_COLORS;
	cout << "grayScale: "
		<< pixel[RED].to_uint() << ","
		<< pixel[GREEN].to_uint() << ","
		<< pixel[BLUE].to_uint() << " = "
		<< temp << endl;*/
	return (
		pixel[RED].to_uint() +
		pixel[GREEN].to_uint() +
		pixel[BLUE].to_uint()
		) / MAX_NO_COLORS;
}

void writeImageToFile(sc_lv<8>* imageData, std::string fileName) {
	ofstream myFile(fileName);
	for (int px = 0; px < IMAGE_SIZE; px++) {
		myFile << imageData[px].to_uint() << endl;
	}
	myFile.close();
	cout << "write done." << endl;
}

sc_lv<8>* selectSegment(sc_lv<8>* pixelsData, int segmentNumber) {
	return &pixelsData[segmentNumber * SEGMENT_SIZE];
}

void printSegment(sc_lv<8>* segment, int rows_count) {
	/*
	for (int y = 0; y < rows_count; y++) {
		for (int x = 0; x < IMAGE_COLS_PX; x++) {
			cout << segment[y * IMAGE_COLS_PX + x].to_uint() << "\t";
		}
		cout << endl;
	}*/
}

int calculateGx(int x, int y, sc_lv<8>* segmentSlot) {
	return 
		segmentSlot[(y + 1) * IMAGE_COLS_PX + x + 1].to_uint() +
		segmentSlot[(y + 1) * IMAGE_COLS_PX + x].to_uint() * 2 +
		segmentSlot[(y + 1) * IMAGE_COLS_PX + x - 1].to_uint() +
		segmentSlot[(y - 1) * IMAGE_COLS_PX + x + 1].to_uint() * (-1) +
		segmentSlot[(y - 1) * IMAGE_COLS_PX + x].to_uint() * (-2) +
		segmentSlot[(y - 1) * IMAGE_COLS_PX + x - 1].to_uint() * (-1);
}

int calculateGy(int x, int y, sc_lv<8>* segmentSlot) {
	return 
		segmentSlot[(y - 1) * IMAGE_COLS_PX + x - 1].to_uint() * (-1) +
		segmentSlot[(y)* IMAGE_COLS_PX + x - 1].to_uint() * (-2) +
		segmentSlot[(y + 1) * IMAGE_COLS_PX + x - 1].to_uint() * (-1) +
		segmentSlot[(y - 1) * IMAGE_COLS_PX + x + 1].to_uint() * (1) +
		segmentSlot[(y)* IMAGE_COLS_PX + x + 1].to_uint() * (2) +
		segmentSlot[(y + 1) * IMAGE_COLS_PX + x + 1].to_uint() * (1);
}

sc_lv<8>* readFile(std::string fileName){
	ifstream channelFile(fileName);
	sc_lv<8>* contents = new sc_lv<8>[IMAGE_SIZE];
	for (int i = 0; i < IMAGE_SIZE; i++) {
		int data;
		channelFile >> data;
		contents[i] = (sc_uint<8>) data;
	}
	std::cout << "done reading " << fileName << std::endl;
	channelFile.close();
	return contents;
}

void applyKernelArrays(sc_lv<8>* segmentSlot, int* Gx, int* Gy, int currentSegmentUnder) {
	for (int y = 1; y < SEGMENT_ROWS_PX + 1; y++) {
		if (currentSegmentUnder == 0 && y < 3)
			continue;
		for (int x = 1; x < IMAGE_COLS_PX - 1; x++) {
			segmentSlot[y * IMAGE_COLS_PX + x] =
				abs(Gx[(y - 1) * IMAGE_COLS_PX + x - 1]) +
				abs(Gy[(y - 1) * IMAGE_COLS_PX + x - 1]);
		}
	}
}

void copyLogicVectors(sc_lv<8>* dst, sc_lv<8>* src, int writeOffset, int writeSize) {
	for (int px = 0; px < writeSize; px++) {
		dst[px] = src[writeOffset + px];
	}
}