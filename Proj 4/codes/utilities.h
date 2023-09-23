#ifndef UTILITIES_HH
#define UTILITIES_HH

#include <systemc.h>
#include "macros.h"
#include <stdlib.h>
#include <fstream>

int grayScale_of(sc_lv<8>* pixel);
void writeImageToFile(sc_lv<8>* imageData, std::string fileName);
sc_lv<8>* selectSegment(sc_lv<8>* pixelsData, int segmentNumber);
void printSegment(sc_lv<8>* segment, int rows_count);
int calculateGx(int x, int y, sc_lv<8>* segmentSlot);
int calculateGy(int x, int y, sc_lv<8>* segmentSlot);
sc_lv<8>* readFile(std::string fileName);
void applyKernelArrays(sc_lv<8>* segmentSlot, int* Gx, int* Gy, int currentSegmentUnder);
void copyLogicVectors(sc_lv<8>* dst, sc_lv<8>* src, int writeOffset, int writeSize);

#endif

