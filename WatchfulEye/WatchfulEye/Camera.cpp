#include <chrono>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include "Camera.h"
#define VIDEO_PATH R"(C:\projectSources\vid_Trim_Trim.mp4)"
#else
#include "Camera.h"
#include <unistd.h>
#include <ncurses.h>
#define VIDEO_PATH R"(../../../../projectSources/vid_Trim_Trim.mp4)"
#endif

using namespace std::chrono;

VideoFrameQueue& frameQueue = VideoFrameQueue::getInstance();

void Camera::start() {

#ifdef __linux__
	initscr();  // Initialize the ncurses library
	keypad(stdscr, TRUE);  // Enable reading of function keys like ESC
	nodelay(stdscr, TRUE);  // Enable non-blocking input
#endif
	VideoCapture cap(VIDEO_PATH);
	if (!cap.isOpened()) {
		Logging::writeTolog(spdlog::level::err, "Error opening video stream");
	}
	long frameCounter = 1;
	double capFrameRate = cap.get(CAP_PROP_FPS);
	const int frameSkip = static_cast<int>(capFrameRate / 3);

	Mat frame;
	Mat prevFrame;
	cap >> frame;
	if (!frame.empty()) {
		pushToTheQueue(cap, frame);
		frameCounter++;
	}

	prevFrame = frame;
	int c = 0;
	while (!frame.empty() && (c != 27)) {
		Mat frame;
		cap >> frame;
		if (frame.empty())
			break;
		if (cmpFrames(prevFrame, frame)) {
			if (frameCounter % frameSkip == 0)
				pushToTheQueue(cap, frame);
			frameCounter++;
		}
		prevFrame = frame;
#ifdef __linux__
		sleep(1);
		c = getch();  // Read a character from the keyboard
#else
		Sleep(1);
		if (_kbhit())
			c = _getch();
#endif
		if (c == 27) {
			string infoChar = "The camera thread exits because the ESC key was pressed.";
			Logging::writeTolog(spdlog::level::info, infoChar);
		}

	}
#ifdef __linux__
	endwin();  // Clean up the ncurses library
#endif
	cap.release();
	destroyAllWindows();
}

void Camera::pushToTheQueue(VideoCapture cap, Mat frame)
{
	VideoFrameData currentFrame;
	currentFrame.timestamp = calcTime();
	currentFrame.timestampVideo = to_string(cap.get(CAP_PROP_POS_MSEC));
	currentFrame.frame = frame;
	frameQueue.push(currentFrame);
}

bool Camera::cmpFrames(Mat prevFrame, Mat frame){
//Calculate absolute difference between the frames.
	Mat diffFrames;
	absdiff(prevFrame, frame, diffFrames);
	if (diffFrames.channels() > 1)
		cvtColor(diffFrames, diffFrames, COLOR_BGR2GRAY);
	//Checks if the frames are different enough
	double amountDiffPixels = static_cast<double>(countNonZero(diffFrames > 0));
	double amountPixelsNeeded = frame.size().area() * INEQUALITY_PERCENTAGES_TO_DECIPHER;
	return amountDiffPixels >= amountPixelsNeeded;
}

string Camera::calcTime() {
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