#include "Glue.h"
#include <iostream>
int main()
{
	string dst;

	class  IGSM_PIN  igsm_pin;
	dst = igsm_pin.get_all_msg("fadsadf");

	for(int i = 0; i < (int)dst.length(); i++)
	{
		printf("%02x ", dst[i] & 0xff);
	}
	std::cout << std::endl;

	class WARN_STAT  warn_stat;
	dst = warn_stat.get_all_msg("eeeeeeeeee");

	for(int i = 0; i < (int)dst.length(); i++)
	{
		printf("%02x ", dst[i] & 0xff);
	}
	std::cout << std::endl;

	return 0;
}
