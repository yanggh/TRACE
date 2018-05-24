#ifndef __JSONCONF__H_
#define __JSONCONF__H_
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "singleton.h"
#include <string>
#include <iostream>

class JsonConf
{
private:
    std::string file;
    rapidjson::Document doc;

    friend  class Singleton<JsonConf>;

    JsonConf():file("/usr/local/trace/conf/trace.json"){  }
    JsonConf(const std::string file):file(file) {  }

    int init();
public:
    std::string getLogDir();
    std::string getSerial1();
    std::string getSerial2();
    std::string getFilePrefix();

    int getUsageRate();
    int getFileSliceSize();
    int getDiff();
    int getTimeout();
    int getDaemonMode();
    void print(std::ostream &out);

    static void initialize(int argc, char **argv);
    static JsonConf &getInstance();
};
#endif
