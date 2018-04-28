#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include "Glue.h"
using namespace std;

string BASE::get_all_msg(string  src)
{
	string dst;

	dst = get_msg_head();
	dst += src;

	return move(dst);
}

string IGSM_PIN::get_msg_head()
{
	string  str = get_igsm_pin_head(len, atp, mt, port, dtype);
	return move(str);	
}

string IGSM_PIN::get_igsm_pin_head(const int len, const int atp, const bool mt, const int port, const TS_TYPE dtype)
{
	cout << "get_igsm_pin_head" << endl;
	vector<uint8_t> sm;

	sm.resize(22);

	uint32_t  frame = 0xffffffff;
	memcpy(sm.data(), &frame, 4);

	uint16_t fra_len = 16+len;
	memcpy(sm.data()+4, &fra_len, 2);

	time_t utc = time(NULL);
	memcpy(sm.data()+6, &utc, 8);

	uint32_t apt_mt = (atp & 0xffffff) | (mt & 0xff00000000);
	memcpy(sm.data()+14, &apt_mt, 4); 

	sm[18] = port;
	if(dtype == PIN)
	{
		sm[19] = 0;
	}
	else if(dtype == UP_SM || dtype == DOWN_SM)
	{
		sm[19] = 1;
	}

	uint16_t dlen = 16 + len;
	memcpy(sm.data()+20, &dlen, 2);

	string res;
	res.insert(res.end(), sm.begin(), sm.end());

	return move(res);
}

string WARN_STAT::get_msg_head()
{
	string str =  get_warn_stat_head(len, atp, mt, dtype);
	return move(str);	
}

string WARN_STAT::get_warn_stat_head(const int len, const int atp, const bool mt, const TS_TYPE dtype)
{
	vector<uint8_t> sm;

	sm.resize(22);

	uint32_t  frame;
	if(dtype == STAT)
	{
		frame = 0xeeeeeeee;
	}
	else if(dtype == WARING)
	{
		frame = 0xdddddddd;
	}
	memcpy(sm.data(), &frame, 4);

	uint16_t fra_len = 16 + len;
	memcpy(sm.data()+4, &fra_len, 2);

	time_t utc = time(NULL);
	memcpy(sm.data()+6, &utc, 8);

	uint32_t apt_mt = (atp & 0xffffff) | (mt & 0xff00000000);
	memcpy(sm.data()+14, &apt_mt, 4); 

	uint16_t spare;
	if(dtype == STAT)
	{ 
		spare = 0xeeee;
	}
	else if(dtype == WARING)
	{
		spare = 0xdddd;
	}
	memcpy(sm.data()+18, &spare, 2);

	uint16_t dlen = 16 + len;
	memcpy(sm.data()+20, &dlen, 2);

	string res;
	res.insert(res.end(), sm.begin(), sm.end());

	return move(res);
}

string get_dst(const Data data)
{
	string dst;
	if(data.type == UP_SM || data.type == DOWN_SM || data.type == PIN)
	{
		class  IGSM_PIN  igsm_pin;
		dst = igsm_pin.get_all_msg(data.data);
	}
	else
	{
		class WARN_STAT warn_stat;
		dst = warn_stat.get_all_msg(data.data);
	}
	return move(dst);
}

