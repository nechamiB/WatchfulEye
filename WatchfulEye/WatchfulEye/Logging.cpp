#include "Logging.h"
using namespace std;

std::vector<std::shared_ptr<spdlog::logger>>Logging::loggers;

spdlog::level::level_enum Logging::getLevel(spdlog::level::level_enum level) {
	switch (level) {
	case spdlog::level::level_enum::trace:
		return spdlog::level::trace;
	case spdlog::level::level_enum::debug:
		return spdlog::level::debug;
	case spdlog::level::level_enum::info:
		return spdlog::level::info;
	case spdlog::level::level_enum::warn:
		return spdlog::level::warn;
	case spdlog::level::level_enum::err:
		return spdlog::level::err;
	case spdlog::level::level_enum::critical:
		return spdlog::level::critical;
	}
	return spdlog::level::debug;
}

void Logging::createLogger(LoggerType type, const std::string loggerName, spdlog::level::level_enum level,
	const string fileName, size_t maxFileSize, size_t maxFiles) {
	std::shared_ptr<spdlog::logger> logger;
	switch (type) {
	case LoggerType::Console:
		logger = spdlog::stdout_color_mt(loggerName);
		break;
	case LoggerType::File:
		logger = spdlog::basic_logger_mt(loggerName, fileName);
		break;
	case LoggerType::DailyFile:
		logger = spdlog::daily_logger_mt(loggerName, fileName);
		break;
	case LoggerType::RotatingFile:
		logger = spdlog::rotating_logger_mt(loggerName, fileName, maxFileSize, maxFiles);
		break;
	}
	loggers.push_back(logger);
	spdlog::set_default_logger(logger);
	spdlog::set_level(getLevel(level));
}

void Logging::closeSpdlog()
{
	spdlog::drop_all();
}

void Logging::writeTolog(spdlog::level::level_enum loggerType, std::string messege)
{
	for (std::shared_ptr<spdlog::logger> logger : loggers)
	{
		if (logger->level() <= loggerType)
		{
			switch (loggerType)
			{
			case spdlog::level::trace:
				spdlog::trace(messege);
				break;
			case spdlog::level::debug:
				spdlog::debug(messege);
				break;
			case spdlog::level::info:
				spdlog::info(messege);
				break;
			case spdlog::level::warn:
				spdlog::warn(messege);
				break;
			case spdlog::level::err:
				spdlog::error(messege);
				break;
			case spdlog::level::critical:
				spdlog::critical(messege);
				break;
			default:
				spdlog::debug(messege);
				break;
			}
		}
	}
}
void Logging::initLogger()
{
	string userAnswer;
	string loggerName;
	bool isChoosed = false;
	cout << "Do you want to choose your logger? (yes/no)";
	cin >> userAnswer;
	while (userAnswer == "yes")
	{
		int type;
		isChoosed = true;
		cout << "Enter your logger name\n";
		cin >> loggerName;
		cout << "Enter your level\n"
			"\t0 - trace\n"
			"\t1 - debug\n"
			"\t2 - info\n"
			"\t3 - warn\n"
			"\t4 - err\n";
		"\t5 - critical\n";
		std::cin >> type;
		type = type >= 0 && type <= 5 ? type : 0;
		spdlog::level::level_enum tyeLev = static_cast<spdlog::level::level_enum>(type);
		cout << "what kind of logger do you want to use\n"
			"\t0 - console\n"
			"\t1 - file\n"
			"\t2 - daily_file\n"
			"\t3 - rotating_file\n";
		cin >> type;
		type = type >= 0 && type <= 3 ? type : 1;
		LoggerType typeLog = static_cast<LoggerType>(type);
		if (typeLog == LoggerType::File || typeLog == LoggerType::DailyFile
			|| typeLog == LoggerType::RotatingFile)
		{
			cout << "Enter your file name" << endl;
			string fileName;
			cin >> fileName;
			if (typeLog == LoggerType::RotatingFile)
			{
				cout << "Enter max size of file, and max number of files" << endl;
				size_t maxFileSize, maxFiles;
				cin >> maxFileSize;
				cin >> maxFiles;
				createLogger(typeLog, loggerName, tyeLev, fileName, maxFileSize, maxFiles);
			}
			else
				createLogger(typeLog, loggerName, tyeLev, fileName);
		}
		else
			createLogger(typeLog, loggerName, tyeLev);

		cout << "Do you want to choose another logger? (yes/no)";
		cin >> userAnswer;
	}
	if (!isChoosed)
		createLogger();
}

