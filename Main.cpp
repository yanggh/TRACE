#include "Reader.h"
#include "Pack.h"
#include "Glue.h"
#include "Record.h"
#include "Daemon_init.h"
using namespace std;

int main(int argc, char** argv)
{
	Daemon_init();
	Reader reader(argv[1]);
	XXX xxx(100, 120, 10);

	Pack  pack;
	while(1)
	{
		Data p = reader.get_data();

		string dst = pack.impl(p);

		if(dst == "invalid")
		{
		    cout << "------------" << endl;
		}
		else
		{
		    xxx.Write(dst);
		}
	}

	return 0;    
}
