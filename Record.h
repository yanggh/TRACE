#ifndef __RECORD__H
#define __RECORD__H
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;
class  XXX
{
private:
	long     limit;
	int      diff;
	time_t   timeout;
	ofstream infile;
	std::vector<std::string>  FileList;

	std::string Open();
	void GetFileVec();
	void Sort();
	void DelOld();
	void AddNewFile(const std::string &filename);
public:
	XXX(const long limit, const int diff, const time_t timeout);

	void Show();
	void Write(const std::string  &val);
};
#endif
