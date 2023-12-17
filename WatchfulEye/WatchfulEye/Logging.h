#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>
#include <vector>
#include <iostream>


enum class LoggerType {
	Console,
	File,
	DailyFile,
	RotatingFile
};

class Logging
{
private:
	spdlog::level::level_enum static getLevel(spdlog::level::level_enum level);
public:
	static std::vector<std::shared_ptr<spdlog::logger>>loggers;
	static void  createLogger(LoggerType type = LoggerType::File,
		const std::string loggerName = "logger",
		spdlog::level::level_enum level = spdlog::level::debug,
		const std::string fileName = "fileLogger.txt",
		size_t maxFileSize = 1024,
		size_t maxFiles = 3);

	void static initLogger();

	void static closeSpdlog();

	static void writeTolog(spdlog::level::level_enum loggerType, std::string messege);
};

