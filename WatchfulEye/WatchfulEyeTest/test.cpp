#include <catch2/catch_test_macros.hpp>
#include "opencv2/opencv.hpp"
#include "../WatchfulEye/WatchfulEye.h"
#include "../WatchfulEye/Sqlite.h"

using namespace cv;

TEST_CASE("check_cmpFrames_function") {
	uint8_t buff[] = { 1,2,3,4,5,6,7,8,9 };
	uint8_t buff1[] = { 11,12,14,15,16,17,18,19,20 };
	uint8_t buff2[] = { 25,26,27,28,29,23,29,35,37 };
	uint8_t buff3[] = { 1,1,3,4,5,6,7,8,9 };
	Mat frames(3, 3, CV_8UC1, buff);
	Mat frames1(3, 3, CV_8UC1, buff1);
	Mat frames2(3, 3, CV_8UC1, buff2);
	Mat frames3(3, 3, CV_8UC1, buff3);

	WatchfulEye watch;
	bool a = watch.cmpFrames(frames, frames1);
	bool b = watch.cmpFrames(frames2, frames3);

	REQUIRE(false == a);
	REQUIRE(true == b);
}

TEST_CASE("check_calcAvg_function") {
	cv::Mat im(3, 3, CV_8UC3);
	im.at<cv::Vec3b>(0, 0) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(0, 1) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(0, 2) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(1, 0) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(1, 1) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(1, 2) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(2, 0) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(2, 1) = cv::Vec3b(10, 123, 4);
	im.at<cv::Vec3b>(2, 2) = cv::Vec3b(10, 123, 4);
	Sqlite sqlite;
	Avgs avg = sqlite.calcAvgs(im, Rect(0, 0, 3, 3));
	REQUIRE(avg.AvgB == 10);
	REQUIRE(avg.AvgG == 123);
	REQUIRE(avg.AvgR == 4);
}