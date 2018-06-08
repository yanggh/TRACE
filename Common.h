#ifndef __COMMON_H_
#define __COMMON_H_

#include <string>
enum  TS_TYPE
{
    UP_SM = 0x80,
    DOWN_SM = 0x81,
    WARING = 0x82,
    STAT = 0x83,
    PIN = 0x84
};

enum PIN_TYPE
{
    CTS = 0,
    DSR = 1,
    DCD = 2
};

enum VOL_TYPE
{
    VOL_H = 1,
    VOL_L = 0,
    CTS_H = 1,
    CTS_L = 0,
    DSR_H = 0x19,
    DSR_L = 0x18,
    DCD_H = 0x09,
    DCD_L = 0x08
};


struct Data {
	uint8_t type;
	std::string data;
};
#endif
