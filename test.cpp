#include <cstring>
#include <string>
#include <cstdio>
using namespace std;
int main(int argc, char** argv)
{
	char tt_1[1024];
	char tt_2[1024];
	bzero(tt_1, 1024);
	bzero(tt_2, 1024);
	//string p = "小区告警asdf信息_20180502062924_201f.tar";
	string p = "20180502062924_201f.tar";
//	sscanf(p.c_str(), "%*[^_]_%[^_]_%[^_]%*s", tt_1, tt_2);
	sscanf(p.c_str(), "%[^_]%*s", tt_1);
	printf("tt_1 = %s\n", tt_1);
}
