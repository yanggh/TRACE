#include "Record.h"
#include <iostream>

int main()
{
	XXX  xxx(100, 1, 2);
	xxx.Write("nihao");
	std::cout << "----------------------" << std::endl;
	xxx.Show();
	std::cout << "----------------------" << std::endl;	
	xxx.Show();
	
	while(1)
	{
		xxx.Write("1234567890");
	}
	return 0;
}
