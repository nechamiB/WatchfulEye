#include "WatchfulEye.h"

queue<VideoFrameData> frameQueue;

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
		double timestamp = frameQueue.front().timestamp;
		frameQueue.pop();

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
		//imshow("img", img);
	}
}

void WatchfulEye::pushToTheQueue(VideoCapture cap,Mat frame)
{
	VideoFrameData currentFrame;
	currentFrame.timestamp = cap.get(CAP_PROP_POS_MSEC);
	currentFrame.frame = frame;
	frameQueue.push(currentFrame);
}




