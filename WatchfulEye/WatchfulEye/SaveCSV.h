#pragma once
#include "opencv2/opencv.hpp"
#include<iostream>
#include<fstream>

using namespace std;
using namespace cv;

#define RChannel 0
#define GChannel 1
#define BChannel 2

struct Avgs {
	int AvgR;
	int AvgG;
	int AvgB;
};

class SaveCSV
{
public:
	void start();
	void SaveDetectionDataToCSV(cv::Mat& subImage, Rect box, double time);
	Avgs calcAvgs(cv::Mat& img, Rect box);
	void close();
private:
	vector<string> initTitle();
	Rect NormalizeBox(const Rect& originalBox, cv::Mat& image);
	vector<string> initRow(Rect box, string  time, string AvgR, string  AvgB, string AvgG);
	void writeRowToCSV(const std::vector<std::string>& row);
};

