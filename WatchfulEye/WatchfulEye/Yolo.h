#pragma once
#include "opencv2/opencv.hpp"
#include <fstream>
#include "Sqlite.h"
#include "Logging.h"

class Yolo
{
public:
#ifdef _WIN32
#define ITEMS_TO_DETECTION_PATH R"(C:\projectSources\classes.txt)"
#else
#define ITEMS_TO_DETECTION_PATH R"(../../../../projectSources/classes.txt)"
#endif
#define ITEMS_TO_DETECTION "car"
#define ROWS 25200
#define CX 0
#define CY 1
#define W 2
#define H 3
#define NUMBER_FOR_ALIGNS  0.5
#define JUMP_TO_THE_NEXT_ROW  85
	const float INPUT_WIDTH = 640.0;
	const float INPUT_HEIGHT = 640.0;
	const float SCORE_THRESHOLD = 0.5;
	const float NMS_THRESHOLD = 0.45;
	const float CONFIDENCE_THRESHOLD = 0.45;

	// Text parameters.
	const float FONT_SCALE = 0.7;
	const int FONT_FACE = cv::HersheyFonts::FONT_HERSHEY_SIMPLEX;
	const int THICKNESS = 1;

	// Colors.
	cv::Scalar BLACK = cv::Scalar(0, 0, 0);
	cv::Scalar BLUE = cv::Scalar(255, 178, 50);
	cv::Scalar YELLOW = cv::Scalar(0, 255, 255);
	cv::Scalar RED = cv::Scalar(0, 0, 255);


	std::vector<std::string>class_list;
	Sqlite sqlite;
	//functions
	Yolo();
	std::vector<cv::Mat> pre_process(cv::Mat& input_image, cv::dnn::Net& net);

	cv::Mat post_process(cv::Mat& input_image,
		std::vector<cv::Mat>& outputs,
		const std::vector<std::string>& class_list,
		string timestamp);

	void detect_objects(cv::Mat& input_image,
		std::vector<cv::Mat>& outputs,
		const std::vector<std::string>& class_list,
		std::vector<int>& class_ids,
		std::vector<float>& confidences,
		std::vector<cv::Rect>& boxes);

	void performNMS(const std::vector<cv::Rect>& boxes,
		const std::vector<float>& confidences,
		std::vector<int>& indices);

	void process_detected_results(cv::Mat& input_image,
		const std::vector<std::string>& class_list,
		const std::vector<int>& class_ids,
		const std::vector<float>& confidences,
		const std::vector<cv::Rect>& boxes,
		string timestamp, std::vector<int> indices);

	void draw_label(cv::Mat& input_image, std::string label, int left, int top);
};