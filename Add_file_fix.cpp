#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctime>
#include <ratio>
#include <chrono>
// put_time example
#include <iostream>     // std::cout
#include <ctime>        // std::time_t, struct std::tm, std::localtime
#include "Add_file_fix.h"

using namespace std;
static const string  Format =  "%Y%m%d%H%M%S";

static const string  pre_igsmr_fix =  "Igsmr_";
static const string  pre_igsmr_sub =  ".tra";
static const string  pre_information_fix =  "Information_";
static const string  pre_information_sub =  ".trb";
static const string  pre_full_fix = "FullTrace_";
static const string  pre_full_sub = ".trc";
static const string  pre_txt_fix = "InfoText_";
static const string  pre_txt_sub = ".trd";

int add_file_fix(const string path)
{
    struct stat s;
    lstat( path.c_str(), &s );
    if( ! S_ISDIR( s.st_mode ) )
    {
        mkdir(path.c_str(), S_IRWXU|S_IRGRP|S_IROTH);
    }

    struct dirent * filename;    // return value for readdir()
    DIR * dir;                   // return value for opendir()
    dir = opendir( path.c_str());
    if( NULL == dir )
    {
        return -1;
    }

    char  strTime[17] = {0};
    using std::chrono::system_clock;
    std::time_t tt = system_clock::to_time_t (system_clock::now());
    struct tm  *ptm = localtime(&tt);
    strftime(strTime, 16, Format.c_str(), ptm);

    string  file;
    while( ( filename = readdir(dir) ) != NULL )
    {
        string path_{path};
        if(path_.back() != '/') path_.push_back('/');
        file = path_ + string(filename->d_name);
        if(file.back() == '_')
        {
            file.pop_back();
            if(file.find(pre_igsmr_fix) != string::npos)
            {
                rename((file+"_").c_str(), (file + pre_igsmr_sub).c_str());
            }

            if(file.find(pre_information_fix) != string::npos)
            {
                rename((file+"_").c_str(), (file + pre_information_sub).c_str());
            }

            if(file.find(pre_full_fix) != string::npos)
            {
                rename((file+"_").c_str(), (file + pre_full_sub).c_str());
            }

            if(file.find(pre_txt_fix) != string::npos)
            {
                rename((file+"_").c_str(), (file + pre_txt_sub).c_str());
            }
        }
    }
    return 0;
}
