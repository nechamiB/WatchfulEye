#pragma once
#include "Yolo.h"
#include "opencv2/opencv.hpp"
#include<iostream>
#include<fstream>
#include<queue>
using namespace std;
using namespace cv;

#define INEQUALITY_PERCENTAGES_TO_DECIPHER 0.007
#define SENSITIVITY_TO_DIFF_BETWEEN_PIXELS 15
#define YOLO_PATH R"(C:\projectSources\yolov5s.onnx)"

//frames with time per-frame
typedef struct videoFrameData {
	double timestamp;
	Mat frame;
}VideoFrameData;

//queue of frames and times
extern queue<VideoFrameData> frameQueue;

//functions
class WatchfulEye{
public:
	bool cmpFrames(Mat prevFrame, Mat frame);
	void detections();
	void pushToTheQueue(VideoCapture cap, Mat frame);
};
