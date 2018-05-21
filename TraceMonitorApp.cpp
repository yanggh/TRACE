#include "Reader.h"
#include "Pack.h"
#include "Glue.h"
#include "Record.h"
#include "JsonConf.h"
#include "Daemon.h"
#include <signal.h>
#include <thread>
using namespace std;

static const string  pre_igsmr_fix =  "Igsmr_";
static const string  pre_igsmr_sub =  ".tra";
static const string  pre_information_fix =  "Information_";
static const string  pre_information_sub =  ".trb";
static const string  pre_full_fix = "FullTrace_";
static const string  pre_full_sub = ".trc";
static const string  pre_txt_fix = "InfoText_";
static const string  pre_txt_sub = ".trd";


namespace {
volatile int g_sigint_flag = 0;

void sigint_process(int signo)
{
    g_sigint_flag = 1;
}

}   // namespace

void  run(const int Port)
{
	struct sigaction    action;
	action.sa_flags = 0;
	action.sa_handler = sigint_process;

	sigaction(SIGINT, &action, NULL);

	std::string dev;
	JsonConf &config = JsonConf::getInstance();
	if(Port == 1)
	{
		dev = config.getSerial1();
	}
	else
	{
		dev = config.getSerial2();
	}	

	Reader reader(dev);

	Record igsmr_record(pre_igsmr_fix, pre_igsmr_sub, Port);
	Record information_record(pre_information_fix,  pre_information_sub, Port);
	Record full_record(pre_full_fix, pre_full_sub, Port);
	Record text_record(pre_txt_fix, pre_txt_sub, Port);

	Pack   pack;
    	while (1)
	{
		if(g_sigint_flag)
		{
			igsmr_record.SIGHANDLE();
			information_record.SIGHANDLE();   
			full_record.SIGHANDLE(); 
			text_record.SIGHANDLE();
			return;
		}

		Data data = reader.get_data();
		string  dst = pack.impl(data);
	
		if(dst != "invalid")
		{
			string dst1 = add_pre_head(data.type, dst);	
			if(data.type == UP_SM || data.type == DOWN_SM || data.type == PIN)
   		    	{
				igsmr_record.Write(dst1);
				time_t  tt = time(NULL);
				struct tm  *tmTime = localtime(&tt);

				char strTime[100];
				strftime(strTime, sizeof(strTime), "[%Y%m%d%H%M%S]: ", tmTime);
				dst += string(strTime);
				text_record.Write(dst);	
			}
			else
			{
				information_record.Write(dst1);
				time_t  tt = time(NULL);
				struct tm  *tmTime = localtime(&tt);

				char strTime[100];
				strftime(strTime, sizeof(strTime), "[%Y%m%d%H%M%S]: ", tmTime);
				dst += string(strTime);
				text_record.Write(dst);	
			}
		}
	}
}

int main(int argc, char **argv)
{
	JsonConf::initialize(argc, argv);
	JsonConf &config = JsonConf::getInstance();
	config.print(cout);
	
	int  daemon = config.getDaemonMode();
	if(daemon == 1)
	{
	    Daemon();
	}

	std::thread  thr1(run, 1);
	std::thread  thr2(run, 2);

	thr1.join();
	thr2.join();
	return 0;
}