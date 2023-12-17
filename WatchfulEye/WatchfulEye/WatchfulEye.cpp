#include "WatchfulEye.h"
#include <chrono>

using namespace std::chrono;

using namespace std;
using namespace cv;

queue<VideoFrameData> frameQueue;

void WatchfulEye::pushToTheQueue(Mat frame)
{
	VideoFrameData currentFrame;
	string timeString = calcTime();
	currentFrame.timestamp = timeString;
	currentFrame.frame = frame;
	frameQueue.push(currentFrame);
	string info = "Push to the frame queue on " + currentFrame.timestamp;
	Logging::writeTolog(spdlog::level::info, info);
}

bool WatchfulEye::cmpFrames(Mat prevFrame, Mat frame) {
	//Calculate absolute difference between the frames.
	Mat diffFrames;
	absdiff(prevFrame, frame, diffFrames);

	if (diffFrames.channels() > 1)
		cvtColor(diffFrames, diffFrames, COLOR_BGR2GRAY);
	//Checks if the frames are different enough
	double amountDiffPixels = static_cast<double>(countNonZero(diffFrames > SENSITIVITY_TO_DIFF_BETWEEN_PIXELS));
	double amountPixelsNeeded = frame.size().area() * INEQUALITY_PERCENTAGES_TO_DECIPHER;

	if (amountDiffPixels >= amountPixelsNeeded)
		return true;
	return false;

}
void WatchfulEye::detections() {
	Yolo yolo;
	if (!frameQueue.empty()) {
		Mat frame;
		frame = frameQueue.front().frame;
		string timestamp = frameQueue.front().timestamp;
		frameQueue.pop();
		string info = "Pop from the frame queue on " + timestamp;
		Logging::writeTolog(spdlog::level::info, info);

		cv::dnn::Net net;
		net = cv::dnn::readNet(YOLO_PATH);
		vector<Mat> detections;
		detections = yolo.pre_process(frame, net);
		Mat img = yolo.post_process(frame, detections, yolo.class_list, timestamp);

		vector<double> layersTimes;
		double freq = getTickFrequency() / 1000;
		double time = net.getPerfProfile(layersTimes) / freq;
		string label = format("Inference time : %.2f ms", time);
		putText(img, label, Point(20, 40), yolo.FONT_FACE, yolo.FONT_SCALE, yolo.RED);
		string infoDetection = "The current frame from the queue is detected, and saved in DB, inference time " + to_string(time);
		Logging::writeTolog(spdlog::level::info, infoDetection);
	}
}

string WatchfulEye::calcTime() {
	// Get the current system time
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	std::tm timeinfo;
#ifdef _WIN32
	localtime_s(&timeinfo, &time);
#else
	localtime_r(&time, &timeinfo);
#endif
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
	std::string formattedTime(buffer);
	return formattedTime;
}