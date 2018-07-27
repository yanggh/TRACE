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

    int  pos_;
    time_t   sign_;
    int      Port;
    int      Port;
    ofstream infile;
    vector<string>  FileList;

    void Open();
    void GetFileVec();
    void Sort();
    void DelOld();
    void AddNewFile(const string &filename);
public:
    Record(const string pre_fix, const string sub_fix, const int port);
    ~Record(){}
    void Show();
    void Write(const std::string  &val);
};

#endif
