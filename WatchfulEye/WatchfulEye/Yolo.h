#pragma once
#include "SaveCSV.h"
#include "opencv2/opencv.hpp"
#include<queue>
#include<fstream>
using namespace std;
using namespace cv;
class Yolo
{
public:
	#define ITEMS_TO_DETECTION_PATH R"(C:\projectSources\classes.txt)"
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
	const int FONT_FACE = FONT_HERSHEY_SIMPLEX;
	const int THICKNESS = 1;
	
	// Colors.
	Scalar BLACK = Scalar(0, 0, 0);
	Scalar BLUE = Scalar(255, 178, 50);
	Scalar YELLOW = Scalar(0, 255, 255);
	Scalar RED = Scalar(0, 0, 255);
	
	
	vector<string>class_list;
	SaveCSV csv;
	
	//functions
	Yolo();
	vector<Mat> pre_process(Mat& input_image, cv::dnn::Net& net);
	Mat post_process(Mat& input_image, vector<Mat>& outputs, 
					const vector<string>& class_list, double timestamp);
	void detect_objects(Mat& input_image, vector<Mat>& outputs, const vector<string>& class_list,
						vector<int>& class_ids, vector<float>& confidences, vector<Rect>& boxes);
	void performNMS(const vector<Rect>& boxes, const vector<float>& confidences, vector<int>& indices);
	void process_detected_results(Mat& input_image, const vector<string>& class_list,
								  const vector<int>& class_ids, const vector<float>& confidences, const vector<Rect>& boxes
								  , double timestamp, vector<int> indices);
	void draw_label(Mat& input_image, string label, int left, int top);
};