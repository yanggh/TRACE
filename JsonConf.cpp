#include "JsonConf.h"
#include <cstdio>
using namespace std;
int JsonConf::init()
{
    FILE* fp = fopen(file.c_str(), "r");
    char json[1024] = {0};
    if(fp == NULL)
    {
	return -1;
    }
    fread(json, 1, 1024, fp);
    fclose(fp);
    fp = NULL;

    data.Parse<0>(json);

    return 0;
}

std::string JsonConf::get_value()
{
    return data["carriorName"].GetString();

}

