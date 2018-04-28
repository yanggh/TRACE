#include "Reader.h"
#include "Pack.h"
//#include "Glue.h"
//#include "Record.h"
using namespace std;
int main(int argc, char** argv)
{
	Reader reader(argv[1]);

	Pack  pack;
	for(int i = 0; i < 300; i++)
	{
		Data p = reader.get_data();

		pack.impl(p);
	}
	
	return 0;    
}
