#pragma once
#include <fstream>
#include <queue>
#include "Yolo.h"
#include "Logging.h"
#include "opencv2/opencv.hpp"

#define INEQUALITY_PERCENTAGES_TO_DECIPHER 0.007
#define SENSITIVITY_TO_DIFF_BETWEEN_PIXELS 15
#ifdef _WIN32
#define YOLO_PATH R"(C:\projectSources\yolov5s.onnx)"

#else
#define YOLO_PATH R"(../../../../projectSources/yolov5s.onnx)"
#endif

//frames with time per-frame
typedef struct videoFrameData {
	string timestamp;
	cv::Mat frame;
}VideoFrameData;

//queue of frames and times
extern std::queue<VideoFrameData> frameQueue;

//functions
class WatchfulEye {
public:
	void pushToTheQueue(cv::Mat frame);
	bool cmpFrames(cv::Mat prevFrame, cv::Mat frame);
	void detections();
	string calcTime();
};
