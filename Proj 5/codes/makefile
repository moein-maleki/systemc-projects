CC = g++ -std=c++14

BUILDS_DIR = builds
SOURCES_DIR = sources
HEADERS_DIR = headers

SYSTEMC_HOME_DIR = /usr/local/systemc-2.3.3
SYSTEMC_AMS_HOME_DIR = /usr/local/systemc-ams-2.3

SYSTEMC_LIB_LINUX=${SYSTEMC_HOME_DIR}/lib-linux64
SYSTEMC_INCLUDE=${SYSTEMC_HOME_DIR}/include
SYSTEMC_AMS_LIB_LINUX=${SYSTEMC_AMS_HOME_DIR}/lib-linux64
SYSTEMC_AMS_INCLUDE=${SYSTEMC_AMS_HOME_DIR}/include

CFLAGS = -I$(HEADERS_DIR) -I$(SYSTEMC_INCLUDE) -I$(SYSTEMC_AMS_INCLUDE) \
	-L$(SYSTEMC_LIB_LINUX) -L$(SYSTEMC_AMS_LIB_LINUX) \
	-lsystemc-ams-2.3 -lsystemc-2.3.3 -lm -Wall -m64 

BINARIES = \
	$(BUILDS_DIR)/main.o $(BUILDS_DIR)/lowpassFilter.o $(BUILDS_DIR)/squareWave.o \
	$(BUILDS_DIR)/sensorFrontend.o $(BUILDS_DIR)/adc.o $(BUILDS_DIR)/tdf2De.o $(BUILDS_DIR)/clockGenerator.o \
	$(BUILDS_DIR)/regizter.o $(BUILDS_DIR)/timer.o $(BUILDS_DIR)/pulseGenerator.o $(BUILDS_DIR)/busInterface.o \
	$(BUILDS_DIR)/processor.o $(BUILDS_DIR)/interconnect.o 

mainSS = \
	$(SOURCES_DIR)/main.cpp $(HEADERS_DIR)/main.h $(HEADERS_DIR)/squareWave.h $(HEADERS_DIR)/lowpassFilter.h \
	$(HEADERS_DIR)/clockGenerator.h	$(HEADERS_DIR)/regizter.h $(HEADERS_DIR)/timer.h $(HEADERS_DIR)/pulseGenerator.h \
	$(HEADERS_DIR)/busInterface.h $(HEADERS_DIR)/processor.h $(HEADERS_DIR)/interconnect.h 

lowPassFilterSS = \
	$(SOURCES_DIR)/lowpassFilter.cpp $(HEADERS_DIR)/lowpassFilter.h	$(HEADERS_DIR)/general.h

squareWaveSS = \
	$(SOURCES_DIR)/squareWave.cpp $(HEADERS_DIR)/squareWave.h $(HEADERS_DIR)/general.h

sensorFrontendSS = \
	$(SOURCES_DIR)/sensorFrontend.cpp $(HEADERS_DIR)/sensorFrontend.h $(HEADERS_DIR)/general.h \
	$(HEADERS_DIR)/lowpassFilter.h $(HEADERS_DIR)/adc.h	$(HEADERS_DIR)/tdf2De.h

adcSS = \
	$(SOURCES_DIR)/adc.cpp $(HEADERS_DIR)/adc.h	$(HEADERS_DIR)/general.h

tdf2DeSS = \
	$(SOURCES_DIR)/tdf2De.cpp $(HEADERS_DIR)/tdf2De.h $(HEADERS_DIR)/general.h

clockGeneratorSS = \
	$(SOURCES_DIR)/clockGenerator.cpp $(HEADERS_DIR)/clockGenerator.h $(HEADERS_DIR)/general.h

regizterSS = \
	$(SOURCES_DIR)/regizter.cpp	$(HEADERS_DIR)/regizter.h $(HEADERS_DIR)/general.h

timerSS = \
	$(SOURCES_DIR)/timer.cpp $(HEADERS_DIR)/timer.h	$(HEADERS_DIR)/general.h

pulseGeneratorSS = \
	$(SOURCES_DIR)/pulseGenerator.cpp $(HEADERS_DIR)/pulseGenerator.h $(HEADERS_DIR)/general.h

busInterfaceSS = \
	$(SOURCES_DIR)/busInterface.cpp $(HEADERS_DIR)/busInterface.h $(HEADERS_DIR)/general.h

processorSS = \
	$(SOURCES_DIR)/processor.cpp $(HEADERS_DIR)/processor.h	$(HEADERS_DIR)/general.h

interconnectSS = \
	$(SOURCES_DIR)/interconnect.cpp	$(HEADERS_DIR)/interconnect.h

$(BUILDS_DIR)/main.o: $(mainSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/main.cpp -o $(BUILDS_DIR)/main.o

$(BUILDS_DIR)/lowpassFilter.o: $(lowPassFilterSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/lowpassFilter.cpp -o $(BUILDS_DIR)/lowpassFilter.o

$(BUILDS_DIR)/squareWave.o: $(squareWaveSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/squareWave.cpp -o $(BUILDS_DIR)/squareWave.o

$(BUILDS_DIR)/sensorFrontend.o: $(sensorFrontendSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/sensorFrontend.cpp -o $(BUILDS_DIR)/sensorFrontend.o

$(BUILDS_DIR)/adc.o: $(adcSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/adc.cpp -o $(BUILDS_DIR)/adc.o

$(BUILDS_DIR)/tdf2De.o: $(tdf2DeSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/tdf2De.cpp -o $(BUILDS_DIR)/tdf2De.o

$(BUILDS_DIR)/clockGenerator.o: $(clockGeneratorSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/clockGenerator.cpp -o $(BUILDS_DIR)/clockGenerator.o

$(BUILDS_DIR)/regizter.o: $(regizterSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/regizter.cpp -o $(BUILDS_DIR)/regizter.o

$(BUILDS_DIR)/timer.o: $(timerSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/timer.cpp -o $(BUILDS_DIR)/timer.o

$(BUILDS_DIR)/pulseGenerator.o: $(pulseGeneratorSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/pulseGenerator.cpp -o $(BUILDS_DIR)/pulseGenerator.o

$(BUILDS_DIR)/busInterface.o: $(busInterfaceSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/busInterface.cpp -o $(BUILDS_DIR)/busInterface.o

$(BUILDS_DIR)/processor.o: $(processorSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/processor.cpp -o $(BUILDS_DIR)/processor.o

$(BUILDS_DIR)/interconnect.o: $(interconnectSS)
	$(CC) $(CFLAGS) -c -g $(SOURCES_DIR)/interconnect.cpp -o $(BUILDS_DIR)/interconnect.o

EXECUTABLE_FILE = executable

all: $(BINARIES)
	$(CC) $(BINARIES) $(CFLAGS) -o $(EXECUTABLE_FILE)	

.PHONY: clean

clean:
	rm -rf $(BUILDS_DIR)/*.o $(EXECUTABLE_FILE) *.vcd
