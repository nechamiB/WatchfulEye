#pragma once
#include <string>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "sqlite-amalgamation/sqlite3.h"

using namespace std;

#ifdef _WIN32
#define DB_PATH R"(C:\projectSources\sqlite\db\watchfulEye.db)"
#else
#define DB_PATH "../../../../projectSources/sqlite/db/watchfulEye.db"
#endif

using namespace std;
using namespace cv;

struct Avgs {
	int AvgR;
	int AvgG;
	int AvgB;
};

#define RChannel 0
#define GChannel 1
#define BChannel 2

class Sqlite
{
public:
	const char* path = DB_PATH;
	int createTable(const char* s);
	int insertData(const char* s, Mat& image, Rect originalBox, string time);
	Rect NormalizeBox(const Rect& originalBox, cv::Mat& image);
	Avgs calcAvgs(cv::Mat& image, Rect box);
	int IsValid(int rc, sqlite3* db, string message);
};

