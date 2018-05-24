#include "JsonConf.h"
#include <cstdio>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;


int JsonConf::init()
{
	using std::string;
	using std::ifstream;

	// read json content into string.
	string      stringFromStream;
	ifstream    in;
	in.open(file, ifstream::in);
	if (!in.is_open())
		return -1;

	string line;
	while (getline(in, line)) {
		stringFromStream.append(line + "\n");
	}
	in.close();

	doc.Parse<0>(stringFromStream.c_str());
	if (doc.HasParseError()) {
		//rapidjson::ParseErrorCode code = doc.GetParseError();
		return -1;
	}

	return 0;
}

std::string JsonConf::getLogDir()
{
	return doc["configParam"]["logdir"].GetString();
}

std::string JsonConf::getSerial1()
{
	return doc["configParam"]["serial1"].GetString();
}

std::string JsonConf::getSerial2()
{
	return doc["configParam"]["serial2"].GetString();
}

std::string JsonConf::getFilePrefix()
{
	return string("xq");
}

int JsonConf::getUsageRate()
{
	return doc["configParam"]["usageRate"].GetInt();
}

int JsonConf::getFileSliceSize()
{
	return doc["configParam"]["filesize"].GetInt();
}

int JsonConf::getDiff()
{
	return doc["configParam"]["diff"].GetInt();
}

int JsonConf::getTimeout()
{
	return doc["configParam"]["timeout"].GetInt();
}

int JsonConf::getDaemonMode()
{
	return doc["configParam"]["daemon"].GetInt();
}

void JsonConf::print(std::ostream &out)
{
	out << "getLogDir() = " << getLogDir() << '\n';
	out << "getSerial1() = " << getSerial1() << '\n';
	out << "getSerial2() = " << getSerial2() << '\n';
	out << "getFileSliceSize() = " << getFileSliceSize() << '\n';
	out << "getDiff() = " << getDiff() << '\n';
	out << "getTimeout() = " << getTimeout() << '\n';
	out << "getDaemonMode() = " << getDaemonMode() << '\n';
	out << "getUsageRate() = " << getUsageRate() << '\n';
}

void JsonConf::initialize(int argc, char **argv)
{
	JsonConf &config = Singleton<JsonConf>::getInstance();
	config.init();
}

JsonConf &JsonConf::getInstance()
{
	return Singleton<JsonConf>::getInstance();
}
