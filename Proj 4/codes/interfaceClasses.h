#ifndef INTERFACES_HH
#define INTERFACES_HH

#include <systemc.h>

class put_pixel_if : virtual public sc_interface {
	public:
		virtual void send_pixel(sc_lv<8>* pixel) = 0;
};

class get_pixel_if : virtual public sc_interface {
	public:
		virtual void get_pixel(sc_lv<8>* pixel) = 0;
};

class put_burst_if : virtual public sc_interface {
public:
	virtual bool send_burst(sc_lv<8>* segment, int burstSize) = 0;
};

class get_burst_if : virtual public sc_interface {
public:
	virtual bool get_burst(sc_lv<8>* segment, int burstSize) = 0;
};

#endif