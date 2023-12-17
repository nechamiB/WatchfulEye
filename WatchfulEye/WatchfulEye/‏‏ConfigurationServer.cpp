#ifdef _WIN32
#include "þþConfigurationServer.h"
#else
#include "ConfigurationServer.h"
#endif

json ConfigurationServer::readConfigFile()
{
	std::ifstream configFile(FILE_CONFIG_PATH);
	if (configFile.is_open()) {
		json config;
		configFile >> config;
		configFile.close();
		return config;
	}
	else {
		Logging::writeTolog(spdlog::level::err, "Unable to open config file.");
		throw std::runtime_error("Unable to open config file.");
	}
}

serverMembers ConfigurationServer::getServerMembers()
{
	json config = readConfigFile();
	serverMembers serverMember;
	serverMember.backendIP = config["backend_ip"];
	serverMember.backendPort = config["backend_port"];
	serverMember.windowsYoloPath = config["windows_yolo_path"];
	serverMember.linuxYoloPath = config["linux_yolo_path"];
	return setLoggingMembers(serverMember);
}

serverMembers ConfigurationServer::setLoggingMembers(serverMembers serverMember) {
	std::ifstream configFile("logging_config.json");
	json loggingConfig;
	if (configFile.is_open()) {
		configFile >> loggingConfig;
		configFile.close();
		serverMember.type = loggingConfig["type"];
		serverMember.loggerName = loggingConfig["loggerName"];
		serverMember.level = loggingConfig["level"];
		serverMember.fileName = loggingConfig["fileName"];
		serverMember.maxFileSize = loggingConfig["maxFileSize"];
		serverMember.maxFiles = loggingConfig["maxFiles"];

		Logging::createLogger(serverMember.type, serverMember.loggerName, serverMember.level,
			serverMember.fileName, serverMember.maxFileSize, serverMember.maxFiles);
	}
	else {
		Logging::initLogger();
	}
	return serverMember;
}

int ConfigurationServer::getQueueMembers()
{
	json config = readConfigFile();
	return config["backend_queue_size"];
}

std::string ConfigurationServer::getSqliteMembers()
{
	json config = readConfigFile();
#ifdef _WIN32
	return config["windows_db_path"];
#else
	return config["linux_db_path"].get<std::string>();
#endif
}

yoloMembers ConfigurationServer::getYoloMembers()
{
	json config = readConfigFile();
	yoloMembers yoloMember;
	yoloMember.itemToDetect = config["item_to_detect"];
#ifdef _WIN32
	yoloMember.detectionPath = config["windows_detections_path"];
#else
	detectionPath = config["linux_detections_path"];
#endif
	return yoloMember;
}
