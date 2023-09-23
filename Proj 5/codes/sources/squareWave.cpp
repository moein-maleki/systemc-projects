#include "squareWave.h"

void squareWave::squareWaveGeneration() {
	int halfPeriod = (SQUARE_WAVE_PERIOD_US) / 2;
	while(1) {
		wait(halfPeriod, SC_US);
		out->write(SQUARE_WAVE_MAX_VALUE);
		wait(halfPeriod, SC_US);
		out->write(SQUARE_WAVE_MIN_VALUE);
	}
}
