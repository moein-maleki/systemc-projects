all = exec.out

BUILD = build
BIN = bin
SOURCE = src

exec.out: $(BUILD)/main.o $(BUILD)/wire.o $(BUILD)/gate.o $(BUILD)/simul.o
	g++ $(BUILD)/main.o $(BUILD)/wire.o $(BUILD)/gate.o $(BUILD)/simul.o -o ca1.out

$(BUILD)/main.o: main.cpp $(BIN)/libs.hpp $(BIN)/simul.hpp
	g++ -c main.cpp -o $(BUILD)/main.o

$(BUILD)/wire.o: $(SOURCE)/wire.cpp $(BIN)/libs.hpp
	g++ -c $(SOURCE)/wire.cpp -o $(BUILD)/wire.o

$(BUILD)/gate.o: $(SOURCE)/gate.cpp $(BIN)/libs.hpp $(BIN)/wire.hpp
	g++ -c $(SOURCE)/gate.cpp -o $(BUILD)/gate.o

$(BUILD)/simul.o: $(SOURCE)/simul.cpp $(BIN)/libs.hpp $(BIN)/gate.hpp $(BIN)/wire.hpp
	g++ -c $(SOURCE)/simul.cpp -o $(BUILD)/simul.o

clean: 
	rm -rf $(BUILD)/ && mkdir -p $(BUILD)