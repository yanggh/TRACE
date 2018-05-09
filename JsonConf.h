#ifndef __JSONCONF__H_
#define __JSONCONF__H_
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <string>
#include <iostream>

class JsonConf
{
private:
    std::string file;
    rapidjson::Document data;
    int init();
public:
    JsonConf():file("demo.txt"){ init(); }
    JsonConf(const std::string file):file(file) { init(); }
    std::string get_value();
};
#endif
