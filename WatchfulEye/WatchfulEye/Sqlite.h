#pragma once
#include <string>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "sqlite-amalgamation/sqlite3.h"
#ifdef _WIN32
#include "þþConfigurationServer.h"
#else
#include "ConfigurationServer.h"
#endif

struct Avgs {
	int AvgR;
	int AvgG;
	int AvgB;
};

#define RChannel 0
#define GChannel 1
#define BChannel 2

class Sqlite {
private:
	static ConfigurationServer configurationServer;
public:
	static std::string path;
	int createTable(std::string path);
	int insertData(std::string path, cv::Mat& image, cv::Rect originalBox, std::string time);
	cv::Rect NormalizeBox(const cv::Rect& originalBox, cv::Mat& image);
	Avgs calcAvgs(cv::Mat& image, cv::Rect box);
	int IsValid(int rc, sqlite3* db, std::string message);
};