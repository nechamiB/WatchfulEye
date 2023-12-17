#include "Server.h"

void Server::detections() {
	VideoFrameQueue& frameQueue = VideoFrameQueue::getInstance();
	frameQueue.setStopServer(false);
	Yolo yolo;
	Sqlite sqlite;
	sqlite.createTable(sqlite.path);

#ifdef __linux__
	initscr();  // Initialize the ncurses library
	keypad(stdscr, TRUE);  // Enable reading of function keys like ESC
	nodelay(stdscr, TRUE);  // Enable non-blocking input
#endif

	int c = 0;
	while (c != 27 || !frameQueue.empty()) {
		VideoFrameData data;
		try {
			data = frameQueue.pop();
		}
		catch (const std::runtime_error& e) {
			return;
		}
		Mat frame;
		frame = data.frame;
		string timestamp = data.timestamp;
		string info = "Pop from the frame queue, The frame that was in at: " + data.timestampVideo;
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

		string infoDetection = "The dequeued frame was detected and saved in the DB, inference time " + to_string(time);
		Logging::writeTolog(spdlog::level::info, infoDetection);

#ifdef __linux__
		c = getch();  // Read a character from the keyboard
#else
		if (_kbhit())
			c = _getch();
#endif

		if (c == 27) {
			frameQueue.setStopServer(true);
			string infoChar = "The server thread exits because the ESC key was pressed.";
			Logging::writeTolog(spdlog::level::info, infoChar);
	}
}

#ifdef __linux__
	endwin();  // Clean up the ncurses library
#endif
}