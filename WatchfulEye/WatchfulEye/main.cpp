#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include "WatchfulEye.h"
#define VIDEO_PATH R"(C:\projectSources\vid_Trim_Trim.mp4)"
#else
#include "WatchfulEye.h"
#include <unistd.h>
#include <ncurses.h>
#define VIDEO_PATH R"(../../../../projectSources/vid_Trim_Trim.mp4)"
#endif

using namespace cv;
using namespace std;

int main()
{
#ifdef __linux__
	initscr();  // Initialize the ncurses library
	keypad(stdscr, TRUE);  // Enable reading of function keys like ESC
	nodelay(stdscr, TRUE);  // Enable non-blocking input
#endif

	Logging::initLogger();
	Yolo yolo;
	Sqlite sqlite;
	sqlite.createTable(sqlite.path);
	WatchfulEye watch;
	//read the video
	VideoCapture cap(VIDEO_PATH);
	if (!cap.isOpened()) {
		Logging::writeTolog(spdlog::level::err, "Error opening video stream");
	}
	Mat frame;
	Mat prevFrame;
	cap >> frame;
	if (!frame.empty()) {
		watch.pushToTheQueue(frame);
		watch.detections();
	}

	prevFrame = frame;
	int c = 0;
	while (!frame.empty() && (c != 27)) {
		Mat frame;
		cap >> frame;
		if (frame.empty())
			break;
		if (watch.cmpFrames(prevFrame, frame)) {
			watch.pushToTheQueue(frame);
			watch.detections();
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
	}

#ifdef __linux__
	endwin();  // Clean up the ncurses library
#endif
	cap.release();
	destroyAllWindows();
	return 0;
}
