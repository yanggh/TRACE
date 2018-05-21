#include "Reader.h"
#include "Pack.h"
#include "Glue.h"
#include "Record.h"
#include "JsonConf.h"
#include "Daemon.h"
#include <signal.h>
using namespace std;

static const string  pre_igsmr_fix =  "Igsmr_";
istatic const string  pre_igsmr_sub =  ".tra";
static const string  pre_xq_fix =  "Information_";
static const string  pre_xq_sub =  ".trb";
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

}

int main(int argc, char** argv)
{
	JsonConf::initialize(argc, argv);
	JsonConf &config = JsonConf::getInstance();
	config.print(cout);
		
	int  daemon = config.getDaemonMode();
	if(daemon == 1)
	{
	    Daemon();
	}

	Reader reader("/dev/ttymxc2");
	signal(SIGINT, &sigint_process);

	Record igsmr_record(pre_igsmr_fix, pre_igsmr_sub, 1);
	Record xq_record(pre_xq_fix,  pre_xq_sub, 1);
	Record full_record(pre_full_fix, pre_full_sub, 1);
	Record text_record(pre_txt_fix, pre_txt_sub, 1);

	Pack   pack;

    	while (1)
	{
		if(g_sigint_flag)
		{
			igsmr_record.SIGHANDLE();
			xq_record.SIGHANDLE();   
			full_record.SIGHANDLE(); 
			text_record.SIGHANDLE(); 
			break;
		}
		Data data = reader.get_data();
		string  dst = pack.impl(data);
	
		if(dst != "invalid")
		{
//			if(data.type == 0x80 || data.type == 0x81)
//			{
//				printf("xxx   ");
//				for(int i = 0; i < dst.size(); i++)
//				{
//					printf("%02x ", dst[i]);
//				}
//			}
			printf("\n");

			string dst1 = add_pre_head(data.type, dst);	
			if(data.type == UP_SM || data.type == DOWN_SM || data.type == PIN)
   		    	{
				igsmr_record.Write(dst1);
			}
			else
			{
				xq_record.Write(dst1);
			}
		}
	}

	return 0;    
}
