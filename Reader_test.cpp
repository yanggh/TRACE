#include "Reader.h"

using namespace std;
int main(int argc, char** argv)
{
	Reader reader(argv[1]);
	
	while(1)
	{
		Data p = reader.get_data();
		cout << "----------------------------" << endl;
		cout << "type == " << std::hex << p.data[0] << endl;
		cout << "len == " << std::hex << p.data[1] << endl;
		cout << "check == " << std::hex << p.data[2] << endl;
		//cout << p.data << endl;
		cout << "----------------------------" << endl;
		//cout << "p->data = " << p.data.length() << endl;
	}
	return 0;
}
