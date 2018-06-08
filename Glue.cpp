#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <chrono>
#include <arpa/inet.h>
#include "Glue.h"
#include "Common.h"
using namespace std;

//string get_warn_stat_head(const uint8_t mt, Data data)
string get_warn_stat_head(const uint8_t mt, const int type, const string data)
{
	vector<uint8_t> sm;

	sm.resize(16);

	uint8_t mt_ = mt;
	uint8_t type_ = (type == WARING ? 0 : 1);

	auto now = chrono::system_clock::now();
	auto now_ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch());

	time_t time_sec = now_ms.count() / 1000;
//	time_sec = ntohl(time_sec);

	uint32_t    time_msec = now_ms.count() % 1000;
//	time_msec = ntohl(time_msec);

	short  time_tz = 8;
//	time_tz = ntohs(time_tz);

	short  time_flag = 1;
//	time_flag = ntohs(time_flag);

	uint32_t    data_len = data.size();
//	data_len = ntohl(data_len);

	sm[0] = mt_;
	sm[1] = type_;

	memcpy(sm.data()+2, &time_sec, 4);
	memcpy(sm.data()+6, &time_msec, 2);
	memcpy(sm.data()+8, &time_tz, 2);
	memcpy(sm.data()+10, &time_flag, 2);
	memcpy(sm.data()+12, &data_len, 4);

	string res;
	res.insert(res.end(), sm.begin(), sm.end());

	return move(res);
}

string get_warn_all_msg(const uint8_t mt, const int type, const string data)
{
	string dst;

	dst =  get_warn_stat_head(mt, type, data);
	dst += data;

	return move(dst);
}

//string get_igsmr_pin_head(const uint8_t mt, const uint8_t mt_s, Data data)
string get_igsmr_pin_head(const uint8_t mt, const uint8_t mt_s, const int type, string data)
{
	cout << "get_igsmr_pin_head" << endl;
	vector<uint8_t> sm;

	uint8_t mt_ = mt;
	uint8_t mt_s_ = mt_s;
	uint8_t type_ = (type == PIN ? 0 : 1);

	auto now = chrono::system_clock::now();
	auto now_ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch());

	time_t time_sec = now_ms.count() / 1000;
//	time_sec = ntohl(time_sec);

	uint32_t    time_msec = now_ms.count() % 1000;
//	time_msec = ntohl(time_msec);

	short  time_tz = 8;
//	time_tz = ntohs(time_tz);

	short  time_flag = 1;
//	time_flag = ntohs(time_flag);

	int    data_len = (type == PIN ? 1 : data.size());
//	data_len = ntohl(data_len);

	sm.resize(17);

	sm[0] = mt_;
	sm[1] = mt_s_;
	sm[2] = type_;

	memcpy(sm.data()+3, &time_sec, 4);
	memcpy(sm.data()+7, &time_msec, 2);
	memcpy(sm.data()+9, &time_tz, 2);
	memcpy(sm.data()+11, &time_flag, 2);
	memcpy(sm.data()+13, &data_len, 4);

	string res;
	res.insert(res.end(), sm.begin(), sm.end());

	return move(res);
}

//string get_igsmr_all_msg(const uint8_t mt, const uint8_t mt_s, Data  data)
string get_igsmr_all_msg(const uint8_t mt, const uint8_t mt_s, const int type, string data)
{
	string dst;

	dst = get_igsmr_pin_head(mt, mt_s, type, data);
	dst += data;

	return move(dst);
}

string add_pre_head(const int type, string data)
{
	string dst;
	if(type == UP_SM || type == DOWN_SM || type == PIN)
	{
		if(type == PIN)
		{
			uint8_t type = data[0];
			uint8_t sig = data[1];
			data.resize(1);

			if(type == CTS)
			{
				data[0] = (sig == VOL_L ? CTS_L : CTS_H);
			}
			else if(type == DSR)
			{
				data[0] = (sig == VOL_L ? DSR_L : DSR_H);
			}
			else if(type == DCD)
			{
				data[0] = (sig == VOL_L ? DCD_L : DCD_H);
			}
		}
		
		if(type == DOWN_SM || type == PIN)
		{
			dst = get_igsmr_all_msg(1, 1, type, data);
		}
		else
		{
			dst = get_igsmr_all_msg(1, 0, type, data);
		}
	}
	else
	{
		dst = get_warn_all_msg(1 , type, data);
	}
	return move(dst);
}

