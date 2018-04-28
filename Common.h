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

struct Data {
	int   type = 0;
	bool  flag = false;
	std::string data;
};
#endif
