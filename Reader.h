#ifndef  __READER_H_
#define  __READER_H_

#include <iostream>
#include <string>
#include <fstream>
#include "Common.h"

class  Reader
{
private:
	int fd;
	int num;
public:
	Reader(const std::string dev);
	~Reader();
	Data get_data();
};
#endif
