#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <chrono>
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

	uint8_t mt_ = 1;
	uint8_t mt_s_ = 1;
	uint8_t type_ = (dtype == PIN ? 0 : 1);

	auto now = chrono::system_clock::now();
	time_t time_now = chrono::system_clock::to_time_t(now);
	int    time_sec = time_now / 1000;
	short  time_min = time_now % 1000;
	short  time_tz = 8;
	short  time_flag = 1;
	int    data_len = (dtype == PIN ? 18 : 17+len);

	sm.resize(17);

	sm[0] = mt_;
	sm[1] = mt_s_;
	sm[2] = type_;

	memcpy(sm.data()+3, &time_sec, 4);
	memcpy(sm.data()+7, &time_min, 2);
	memcpy(sm.data()+9, &time_tz, 2);
	memcpy(sm.data()+11, &time_flag, 2);
	memcpy(sm.data()+13, &data_len, 4);

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

	sm.resize(16);

	uint8_t mt_ = 1;
	uint8_t type_ = (dtype == WARING ? 0 : 1);

	auto now = chrono::system_clock::now();
	time_t time_now = chrono::system_clock::to_time_t(now);
	int    time_sec = time_now / 1000;
	short  time_min = time_now % 1000;
	short  time_tz = 8;
	short  time_flag = 1;
	int    data_len = 17+len;

	sm[0] = mt_;
	sm[1] = type_;

	memcpy(sm.data()+2, &time_sec, 4);
	memcpy(sm.data()+6, &time_min, 2);
	memcpy(sm.data()+8, &time_tz, 2);
	memcpy(sm.data()+10, &time_flag, 2);
	memcpy(sm.data()+12, &data_len, 4);

	string res;
	res.insert(res.end(), sm.begin(), sm.end());

	return move(res);
}

string get_dst(Data data)
{
	string dst;
	if(data.type == UP_SM || data.type == DOWN_SM || data.type == PIN)
	{
		class  IGSM_PIN  igsm_pin;
		if(data.type == PIN)
		{
			uint8_t type = data.data[0];
			uint8_t sig = data.data[1];
			data.data.resize(1);

			if(type == CTS)
			{
				data.data[0] = (sig == VOL_L ? CTS_L : CTS_H);
			}
			else if(type == DSR)
			{
				data.data[0] = (sig == VOL_L ? DSR_L : DSR_H);
			}
			else if(type == DCD)
			{
				data.data[0] = (sig == VOL_L ? DCD_L : DCD_H);
			}
		}
		dst = igsm_pin.get_all_msg(data.data);
	}
	else
	{
		class WARN_STAT warn_stat;
		dst = warn_stat.get_all_msg(data.data);
	}
	return move(dst);
}

