#ifndef __RECORD__H
#define __RECORD__H
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;
class  Record
{
private:
	string   newfile;

	string   pre_fix;
	string   sub_fix;

	int	 pos_;
	time_t   sign_;
	int      Port; 
	ofstream infile;
	std::vector<std::string>  FileList;

	void Open();
	void GetFileVec();
	void Sort();
	void DelOld();
	void AddNewFile(const std::string &filename);
public:
	Record(const string pre_fix, const string sub_fix, const int port);
	~Record(){}
	void SIGHANDLE();
	void Show();
	void Write(const std::string  &val);
};
#endif
