#include <strings.h>
#include <string>
#include <iostream>
using namespace std;

//static const string  Parse =    "%*[^_]_%[^_]%*s";
static const string  Parse =    "%*[^_]_%*[^_]_%[^._]%*s";
static const string  pFormat =  "%Y%m%d%H%M%S";
int main(int argc, char** argv)
{

    string file1{argv[1]};
    string file2{argv[2]};
    struct tm tm_f;
    time_t  tt_1;
    time_t  tt_2;

    char  start1[256];
    char  start2[256];

    bzero(start1, 256);
    bzero(start2, 256);

    sscanf(file1.c_str(), Parse.c_str(), start1);
    sscanf(file2.c_str(), Parse.c_str(), start2);

    strptime(start1, pFormat.c_str(), &tm_f);
    tt_1 = mktime(&tm_f);

    cout << start1 << endl;
    strptime(start2, pFormat.c_str(), &tm_f);
    cout << start2 << endl;
    tt_2 = mktime(&tm_f);

    return tt_1 > tt_2;
}
