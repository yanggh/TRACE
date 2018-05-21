#include <iostream>
#include <cstring>
#include <vector>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdint>
#include "JsonConf.h"
#include "Record.h"
using namespace std;

static const string  dir_name = "/usr/local/trace/record/";
static const string  pFormat =  "%Y%m%d%H%M%S";
static const string  Format =   pFormat + "_";
static const string  Parse =    "%*[^_]_%[^_]%*s";


static string filename(const int diff = 0)
{
	time_t  tt = time(NULL);
	tt -= diff * 60;
	struct tm  *tmTime = localtime(&tt);

	char strTime[100];
	strftime(strTime, sizeof(strTime), Format.c_str(), tmTime);

	string file(strTime);
	return file;
}

static string Add_Over_Time(const string &filename, const int Port, const string &suf_fix)
{
	time_t  tt = time(NULL);
	struct tm  *tmTime = localtime(&tt);

	char strTime[100];
	strftime(strTime, sizeof(strTime), Format.c_str(), tmTime);

	string file = filename + string(strTime) + to_string(Port) + suf_fix;

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

static void Del(const std::string &filename)
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

	return tt_1 > tt_2; 
}

void  Record::Open()
{
	newfile = pre_fix + filename();
	
	string path  = Get_all_path(newfile);
	infile.open(path.c_str());
	if(!infile.is_open())
	{
		exit(EXIT_FAILURE);
	}
}

void Record::Sort()
{
		std::sort(FileList.begin(), FileList.end(), &CComp);
//		std::sort(FileList.begin(), FileList.end(), 
//			[&] (const std::string &a, const std::string &b) -> bool
//			{ return this->CComp(a, b); });
	//	struct MyComp {
	//		Record *p;
	//		bool (Record::*ptr)(const std::string &file1,  const std::string &file2);
	//		bool operator () (const std::string &file1,  const std::string &file2) 
	//		{
	//			return (p->*ptr)(file1, file2);
	//		}
	//	};
	//	MyComp comp;
	//	comp.p = this;
	//	comp.ptr = &Record::CComp;
	//	std::sort(FileList.begin(), FileList.end(), comp);
//		std::sort(FileList.begin(), FileList.end(), std::bind(&Record::CComp, this, std::placeholders::_1, std::placeholders::_2));
}

void Record::AddNewFile(const std::string &filename)
{
	FileList.insert(FileList.begin(), filename);
}

Record::Record(const string pre_fix, const string sub_fix, const int port):pre_fix(pre_fix), sub_fix(sub_fix), Port(port)
{
	GetFileVec();	
	Sort();
	Open();
}


void Record::SIGHANDLE()
{
	string  outfile;
	string  src_path;
	string  dst_path;

	if(infile.is_open())
	{
		infile.close();
	}

	outfile = Add_Over_Time(newfile, Port, sub_fix);
	src_path = Get_all_path(newfile);
	dst_path = Get_all_path(outfile);
	rename(src_path.c_str(), dst_path.c_str());
}

void Record::Show()
{
	std::vector<std::string>::iterator iter;

	for(iter = FileList.begin(); iter != FileList.end(); iter++)
	{
		cout << *iter << endl;
	}
}

void  Record::Write(const std::string  &val)
{
	static  int  pos = 0;
	static  time_t  sign = time(NULL);

	JsonConf &config = JsonConf::getInstance();
	int  limit = config.getFileSliceSize();
	int  timeout = config.getTimeout();

	if (limit <= pos)
	{
		cout << "limit = " << limit << ", pos = " << pos << endl;
		string  outfile;
		string  src_path;
		string  dst_path;

		infile.close();
		outfile = Add_Over_Time(newfile, Port, sub_fix);
		src_path = Get_all_path(newfile);
		dst_path = Get_all_path(outfile);

		DelOld();
		AddNewFile(outfile);

		rename(src_path.c_str(), dst_path.c_str());
		Open();
		pos = 0;
	}
	else  if((timeout > 0) && timeout <= (time(NULL) - sign))
	{
		cout << "timeout = " << timeout << endl;
		string  outfile;
		string  src_path;
		string  dst_path;

		infile.close();
		outfile = Add_Over_Time(newfile, Port, sub_fix);
		src_path = Get_all_path(newfile);
		dst_path = Get_all_path(outfile);

		DelOld();
		AddNewFile(outfile);

		rename(src_path.c_str(), dst_path.c_str());
		Open();
		pos = 0;
		sign = time(NULL);
	}
	 
	pos += val.size();
	infile << val << flush;	
}

void Record::GetFileVec()
{
	struct stat s;
	lstat( dir_name.c_str() , &s );
	if( ! S_ISDIR( s.st_mode ) )
	{
		cout<<"dir_name is not a valid directory !"<<endl;
		mkdir(dir_name.c_str(), S_IRWXU|S_IRGRP|S_IROTH);
		cout<<"create " << dir_name << endl;
	}
	
	struct dirent * filename;    // return value for readdir()
 	DIR * dir;                   // return value for opendir()
	dir = opendir( dir_name.c_str() );
	if( NULL == dir )
	{
		cout<<"Can not open dir "<<dir_name<<endl;
		return;
	}
	
	string  spli = string("_")+to_string(Port)+string(".");
	while( ( filename = readdir(dir) ) != NULL )
	{
		if( strcmp( filename->d_name , "." ) == 0 || 
			strcmp( filename->d_name , "..") == 0    )
			continue;

		if(strstr(filename->d_name, pre_fix.c_str()) == NULL)
			continue;

		if(strstr(filename->d_name, spli.c_str()) == NULL)
			continue;

		FileList.push_back(filename->d_name);
	}
} 

void Record::DelOld()
{
	JsonConf &config = JsonConf::getInstance();
	int  diff = config.getDiff();

	string  file = filename(diff);

	while(!FileList.empty())
	{
		if(!CComp(FileList.back(), "test_" + file))
		{
			Del(FileList.back());
			FileList.pop_back();
		}
		else
		{
			break;
		}
	}
}
