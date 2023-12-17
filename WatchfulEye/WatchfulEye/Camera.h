#define INEQUALITY_PERCENTAGES_TO_DECIPHER 0.007
#define SENSITIVITY_TO_DIFF_BETWEEN_PIXELS 15

#include "Queue.h"
#include "Yolo.h"
#include "opencv2/opencv.hpp"

class Camera {
public:
	void start();
	void pushToTheQueue(VideoCapture cap, cv::Mat frame);
	bool cmpFrames(Mat prevFrame, Mat frame);
	string calcTime();
};
