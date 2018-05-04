#ifndef  __GLUE_H_
#define  __GLUE_H_
#include <string>
#include "Common.h"
using namespace std;

class   BASE
{
private:
	virtual string get_msg_head() = 0;
public:
	virtual string get_all_msg(string  src);
};

class  IGSM_PIN: public BASE
{
private:
	int len;
	int atp;
	int mt;
	int port;
	TS_TYPE dtype;

	virtual string get_msg_head();

	string  get_igsm_pin_head(const int len, const int atp, const bool mt, const int port, const TS_TYPE dtype);

public:
	IGSM_PIN(const int len1, const int atp1, const bool mt1, const int port1, const TS_TYPE dtype1):len(len1),atp(atp1),mt(mt1),port(port1),dtype(dtype1)
	{
	}

	IGSM_PIN():len(0), atp(0), mt(0), dtype(UP_SM)
	{
	}

};

class  WARN_STAT: public BASE
{
private:
	int len;
	int atp;
	int mt;
	TS_TYPE dtype;

	virtual string get_msg_head();
	string get_warn_stat_head(const int len, const int atp, const bool mt, const TS_TYPE dtype);

public:
	WARN_STAT(const int len1, const int atp1, const bool mt1, const TS_TYPE dtype1):len(len1),atp(atp1),mt(mt1), dtype(dtype1)
	{
	}

	WARN_STAT():len(0), atp(0), mt(0), dtype(STAT)
	{
	}

};

string get_dst(Data data);
#endif
