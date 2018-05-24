#include <iostream>
#include "JsonConf.h"

using namespace std;
int main(int argc, char** argv)
{
	JsonConf::initialize(argc, argv);
	JsonConf &config = JsonConf::getInstance();
	config.print(cout);
	return 0;
}

