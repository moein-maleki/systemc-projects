#include "regizter.h"

void regizter::registering() {
    while(1) {
        temp = in;
        wait();
    }
}

void regizter::tristate() {
    out.write(REGISTER_LOGIC_Z_VALUE);
    while(1) {
        if(outEnable == SC_LOGIC_1) {
            out.write(temp.read());
        }
        else {
            out.write(REGISTER_LOGIC_Z_VALUE);
        }
        wait();
    }
}
