#include "./bin/libs.hpp"
#include "./bin/simul.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    simul sim(argv[VERILOG_FILE_NAME], argv[TEST_FILE_NAME], TIME_RESOLUTION);
    sim.run();
}
