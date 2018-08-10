#ifndef __RECORD__H
#define __RECORD__H
#include <fstream>
#include <mutex>
#include <vector>
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
    ofstream infile;

    static mutex   mtx;

    void   Open();
    static void DelOld();
    static vector<string>  flist;
    static void AddNewFile(const string &filename);
public:
    static void Getlist();
    static void Show();
    Record(const string pre_fix, const string sub_fix, const int port);
    ~Record(){}
    void Write(const string  &val);
};

#endif
