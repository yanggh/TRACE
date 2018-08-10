#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>
#include "Getdiskinfo.h"
#include "JsonConf.h"
#include "Record.h"

using namespace std;

static const string  dir_name = "/usr/local/trace/record/";
static const string  pFormat =  "%Y%m%d%H%M%S";
static const string  Format =   pFormat + "_";
static const string  Parse =    "%*[^_]_%*[^_]_%[^._]%*s";

vector<string>  Record::flist;
mutex           Record::mtx;

static string filename(const int diff = 0)
{
    time_t  tt = time(NULL);
    tt -= diff;
    struct tm  *tmTime = localtime(&tt);

    char strTime[100];
    strftime(strTime, sizeof(strTime), Format.c_str(), tmTime);

    string file(strTime);
    return file;
}

static string Add_Over_Time(const string &filename, const string &sub_fix)
{
    time_t  tt = time(NULL);
    struct tm  *tmTime = localtime(&tt);

    char strTime[100];
    strftime(strTime, sizeof(strTime), pFormat.c_str(), tmTime);

    //string file = filename + string(strTime) + to_string(Port) + sub_fix;
    string file = filename + string(strTime) + sub_fix;

    return file;
}

static string Get_all_path(const string filename)
{
    string filepath;
    if(dir_name.find_last_of("/") == string::npos)
        filepath = dir_name + "/" + filename;
    else
        filepath = dir_name + filename;

    return filepath;
}

static void Del(const string &filename)
{
    string filepath = Get_all_path(filename);

    remove(filepath.c_str());

    cout << "Del filename is: " << filepath << endl;
}

static bool CComp(const string &file1,  const string &file2)
{
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

    strptime(start2, pFormat.c_str(), &tm_f);
    tt_2 = mktime(&tm_f);

    return (tt_1 > tt_2);
}

void  Record::Open()
{
    newfile = pre_fix + to_string(Port) + "_" + filename();

    string path  = Get_all_path(newfile);
    infile.open(path.c_str());
    if(!infile.is_open())
    {
        exit(EXIT_FAILURE);
    }
    pos_ = 0;
    sign_ = time(NULL);
}

void Record::AddNewFile(const string &filename)
{
    mtx.lock();
//  flist.push_back(filename);
    flist.insert(flist.begin(), filename);
    mtx.unlock();
}

Record::Record(const string pre_fix, const string sub_fix, const int port):pre_fix(pre_fix), sub_fix(sub_fix), Port(port)
{
    Open();
}

void Record::Show()
{
    for(auto& it : flist)
        cout << it << endl;
    cout << "-------------------\n";
}

void  Record::Write(const string  &val)
{
    JsonConf &config = JsonConf::getInstance();
    int  limit = config.getFileSliceSize();
    int  timeout = config.getTimeout();

    if (limit <= pos_)
    {
        cout << "limit = " << limit << ", pos_ = " << pos_ << endl;
        string  outfile;
        string  src_path;
        string  dst_path;

        infile.close();
        outfile = Add_Over_Time(newfile, sub_fix);
        src_path = Get_all_path(newfile);
        dst_path = Get_all_path(outfile);

        DelOld();
        AddNewFile(outfile);

        rename(src_path.c_str(), dst_path.c_str());
        Open();
    }
    else  if((timeout > 0) && timeout <= (time(NULL) - sign_))
    {
        cout << "timeout = " << timeout << endl;
        string  outfile;
        string  src_path;
        string  dst_path;

        infile.close();
        outfile = Add_Over_Time(newfile, sub_fix);
        src_path = Get_all_path(newfile);
        dst_path = Get_all_path(outfile);

        DelOld();
        AddNewFile(outfile);

        rename(src_path.c_str(), dst_path.c_str());
        Open();
    }

    pos_ += val.size();
    infile << val << flush;
}

void Record::Getlist()
{
    struct stat s;
    lstat( dir_name.c_str() , &s );
    if( ! S_ISDIR( s.st_mode ) )
    {
        mkdir(dir_name.c_str(), S_IRWXU|S_IRGRP|S_IROTH);
    }

    struct dirent * filename;    // return value for readdir()
    DIR * dir;                   // return value for opendir()
    dir = opendir( dir_name.c_str() );
    if( NULL == dir )
    {
        cout<<"Can not open dir "<<dir_name<<endl;
        return;
    }

    while( ( filename = readdir(dir) ) != NULL )
    {
        if( strcmp(filename->d_name , "." ) == 0 ||
                strcmp( filename->d_name , "..") == 0)
            continue;

        if(strstr(filename->d_name, ".tr") == NULL )
            continue;
        flist.insert(flist.begin(), filename->d_name);
    }

//    flist.sort(CComp);
    if(flist.size() > 0)
    {
        std::sort(flist.begin(), flist.end(), &CComp);
        cout << "front ===== " << flist.front() << endl;
        cout << "back ===== " << flist.back() << endl;
    }
}

void Record::DelOld()
{
    mtx.lock();
    JsonConf &config = JsonConf::getInstance();
    int  diff = config.getDiff();

    string  file = filename(diff);
    while(!flist.empty())
    {
    cout << "file == " << file << ", list = " << flist.size() << endl;
    cout << "flist.front = "  << flist.front() << endl;
    cout << "flist.back = "  << flist.back() << endl;
        if(!CComp(flist.back(), "test_1_" + file))
        {
            cout << "timeout " << endl;
            cout << "file = " << file << endl;

            Del(flist.back());
            flist.pop_back();
        }
        else
        {
            break;
        }
    }

    int  conf_percent = config.getUsageRate();
    int  percent = 0;
    while(!flist.empty())
    {
        percent = get_disk_percent("/");
        if(percent >= conf_percent)
        {
            cout << "conf_percent = " << conf_percent << endl;
            cout << "percent = " << percent << endl;

            Del(flist.back());
            flist.pop_back();
        }
        else
        {
            break;
        }
    }
    mtx.unlock();
}
