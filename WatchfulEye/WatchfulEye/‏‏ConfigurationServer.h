#ifndef CONFIGURATIONSERVER_H
#define CONFIGURATIONSERVER_H
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "Logging.h"
#include "json.hpp"
#ifdef _WIN32
#define FILE_CONFIG_PATH R"(..\..\WatchfulEye\config.json)"
#else
#define FILE_CONFIG_PATH R"(../../../../WatchfulEye/config.json)"
#endif

using json = nlohmann::json;

typedef struct serverMembers {
	std::string backendPort;
	std::string backendIP;
	LoggerType type;
	std::string loggerName;
	spdlog::level::level_enum level;
	std::string fileName;
	size_t maxFileSize;
	size_t maxFiles;
	std::string windowsYoloPath;
	std::string linuxYoloPath;
}serverMembers;

typedef struct yoloMembers {
	std::string detectionPath;
	std::string itemToDetect;
} yoloMembers;

class ConfigurationServer {
public:
	json readConfigFile();
	//reads configuration data from a JSON file and sets variables 
	serverMembers getServerMembers();
	//Initialize the logging configuration based on another JSON file-if it doesn't exist.
	//if it exist, reads configuration data from logging JSON
	serverMembers setLoggingMembers(serverMembers serverMember);
	int getQueueMembers();
	std::string getSqliteMembers();
	yoloMembers getYoloMembers();
};
#endif
