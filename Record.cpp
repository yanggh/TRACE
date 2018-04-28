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

#include "Record.h"
using namespace std;

static string  dir_name = "testing/";
static string  pre_fix =  "asdf";
static string  Format =   "%Y%m%d%H%M%S";
static string  pFormat =  pre_fix + Format;

static string filename(const int diff = 0)
{
	time_t  tt = time(NULL);
	tt -= diff * 60;
	struct tm  *tmTime = localtime(&tt);

	char strTime[100];
	strftime(strTime, sizeof(strTime), Format.c_str(), tmTime);

	string filename = pre_fix + string(strTime);
	return filename;
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

	strptime(file1.c_str(), pFormat.c_str(), &tm_f);
	tt_1 = mktime(&tm_f);

	strptime(file2.c_str(), pFormat.c_str(), &tm_f);
	tt_2 = mktime(&tm_f);

	return tt_1 > tt_2; 
}

string XXX::Open()
{
	std::string file = filename();

	string path  = Get_all_path(file);
	infile.open(path.c_str());
	if(!infile.is_open())
	{
		exit(EXIT_FAILURE);
	}

	DelOld();
	AddNewFile(file);

	return file;
}

void XXX::Sort()
{
		std::sort(FileList.begin(), FileList.end(), &CComp);
//		std::sort(FileList.begin(), FileList.end(), 
//			[&] (const std::string &a, const std::string &b) -> bool
//			{ return this->CComp(a, b); });
	//	struct MyComp {
	//		XXX *p;
	//		bool (XXX::*ptr)(const std::string &file1,  const std::string &file2);
	//		bool operator () (const std::string &file1,  const std::string &file2) 
	//		{
	//			return (p->*ptr)(file1, file2);
	//		}
	//	};
	//	MyComp comp;
	//	comp.p = this;
	//	comp.ptr = &XXX::CComp;
	//	std::sort(FileList.begin(), FileList.end(), comp);
//		std::sort(FileList.begin(), FileList.end(), std::bind(&XXX::CComp, this, std::placeholders::_1, std::placeholders::_2));
}

void XXX::AddNewFile(const std::string &filename)
{
	FileList.insert(FileList.begin(), filename);
}

XXX::XXX(const long limit, const int diff, const time_t  timeout):limit(limit), diff(diff), timeout(timeout)
{
	GetFileVec();	
	Sort();
	Open();
};

void XXX::Show()
{
	std::vector<std::string>::iterator iter;

	for(iter = FileList.begin(); iter != FileList.end(); iter++)
	{
		cout << *iter << endl;
	}
}

void  XXX::Write(const std::string  &val)
{
	static  time_t  sign = time(NULL);

	if (limit * 1024 * 1024 <= infile.tellp())
	{
		infile.close();
		Open();
	}
	else  if(timeout > 0 && timeout <= time(NULL) - sign)
	{
		infile.close();
		Open();
		sign = time(NULL);
	}
	 
	infile << val;	
}

void XXX::GetFileVec()
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
	
	while( ( filename = readdir(dir) ) != NULL )
	{
		if( strcmp( filename->d_name , "." ) == 0 || 
			strcmp( filename->d_name , "..") == 0    )
			continue;

		if(strstr(filename->d_name, pre_fix.c_str()) == NULL)
			continue;

		FileList.push_back(filename->d_name);
	}
} 

void XXX::DelOld()
{
	string  file = filename(diff);

	while(!FileList.empty())
	{
		if(!CComp(FileList.back(), file))
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
