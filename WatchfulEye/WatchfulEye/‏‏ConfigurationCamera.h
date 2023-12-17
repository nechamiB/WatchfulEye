#pragma once
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

typedef struct cameraMembers {
	float cameraThreshold;
	std::string cameraIP;
	std::string cameraPort;
	int cameraDiffPixels;
	std::string windowsVideoPath;
	std::string linuxVideoPath;
} cameraMembers;

class ConfigurationCamera {
public:
	//reads configuration data from a JSON file and sets variables 
	cameraMembers getCameraMembers();
};
