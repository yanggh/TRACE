#include "Reader.h"
#include "Common.h"

using namespace std;


int main(int argc, char** argv)
{
	Reader reader;
	while(1)
	{
		Data data = reader.get_data();
		
		printf("reader.get_data = %s\n",  data.data.c_str());
		printf("reader.type = %x\n", data.type);
	}

	return 0;    
}
