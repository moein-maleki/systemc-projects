all = exec.out

BUILD = build
BIN = bin
SOURCE = src

exec.out: \
	$(BUILD)/main.o \
	$(BUILD)/wire.o \
	$(BUILD)/event.o \
	$(BUILD)/gate.o \
	$(BUILD)/tree.o \
	$(BUILD)/simul.o
	g++ $(BUILD)/main.o \
	$(BUILD)/wire.o \
	$(BUILD)/event.o \
	$(BUILD)/tree.o \
	$(BUILD)/gate.o \
	$(BUILD)/simul.o -o ca1.out

$(BUILD)/main.o: \
	main.cpp \
	$(BIN)/libs.hpp \
	$(BIN)/simul.hpp
	g++ -c main.cpp -o $(BUILD)/main.o

$(BUILD)/wire.o: \
	$(SOURCE)/wire.cpp \
	$(BIN)/libs.hpp \
	$(BIN)/event.hpp
	g++ -c $(SOURCE)/wire.cpp -o $(BUILD)/wire.o

$(BUILD)/gate.o: \
	$(SOURCE)/gate.cpp \
	$(BIN)/libs.hpp \
	$(BIN)/wire.hpp
	g++ -c $(SOURCE)/gate.cpp -o $(BUILD)/gate.o

$(BUILD)/event.o: \
	$(SOURCE)/event.cpp \
	$(BIN)/libs.hpp \
	$(BIN)/wire.hpp
	g++ -c $(SOURCE)/event.cpp -o $(BUILD)/event.o

$(BUILD)/simul.o: \
	$(SOURCE)/simul.cpp \
	$(BIN)/libs.hpp \
	$(BIN)/gate.hpp \
	$(BIN)/event.hpp \
	$(BIN)/wire.hpp \
	$(BIN)/tree.hpp
	g++ -c $(SOURCE)/simul.cpp -o $(BUILD)/simul.o

$(BUILD)/tree.o: \
	$(SOURCE)/tree.cpp \
	$(BIN)/libs.hpp \
	$(BIN)/gate.hpp \
	$(BIN)/wire.hpp
	g++ -c $(SOURCE)/tree.cpp -o $(BUILD)/tree.o

clean: 
	rm -rf $(BUILD)/ && mkdir -p $(BUILD)