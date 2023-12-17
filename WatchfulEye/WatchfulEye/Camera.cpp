#include "Camera.h"

using namespace std;
using namespace cv;

//Initialize the static variables.are used to get the configurations
ConfigurationCamera Camera::configurationCamera;
cameraMembers Camera::cameraMember = configurationCamera.getCameraMembers();

void Camera::start() {
	InitializeChannelAndStub();
#ifdef __linux__
	initscr();  // Initialize the ncurses library
	keypad(stdscr, TRUE);  // Enable reading of function keys like ESC
	nodelay(stdscr, TRUE);  // Enable non-blocking input
#endif

#ifdef _WIN32
	VideoCapture cap(this->cameraMember.windowsVideoPath);
#else
	VideoCapture cap(this->cameraMember.linuxVideoPath);
#endif
	//read the video
	try {
		if (!cap.isOpened()) {
			throw std::runtime_error("Error opening video stream");
		}
	}
	catch (const std::exception& e) {
		Logging::writeTolog(spdlog::level::err, e.what());
	}
	//Variables to use to ensure that only 3fps will be sent to the server
	long frameCounter = 1;
	double capFrameRate = cap.get(CAP_PROP_FPS);
	const int frameSkip = static_cast<int>(capFrameRate / 3);

	Mat frame;
	Mat prevFrame;
	cap >> frame;
	if (!frame.empty()) {
		string timestamp = calcTime();
		sendToServer(cap, frame, timestamp);
		frameCounter++;
	}

	prevFrame = frame;
	int c = 0;
	while (!frame.empty() && (c != 27/*ESC*/)) {
		Mat frame;
		cap >> frame;
		if (frame.empty())
			break;

		if (frameCounter % frameSkip == 0) {
			if (cmpFrames(prevFrame, frame)) {
				string timestamp = calcTime();
				try { sendToServer(cap, frame, timestamp); }
				catch (const std::runtime_error& e) { return; }				
			}
		}
		frameCounter++;

		frameCounter = (frameCounter == capFrameRate) ? 1 : frameCounter;
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
			string infoChar = "The camera exits because the ESC key was pressed.";
			Logging::writeTolog(spdlog::level::info, infoChar);
		}

	}
#ifdef __linux__
	endwin();  // Clean up the ncurses library
#endif
	cap.release();
	destroyAllWindows();
}

bool Camera::cmpFrames(Mat prevFrame, Mat frame) {
	//Calculate absolute difference between the frames.
	Mat diffFrames;
	absdiff(prevFrame, frame, diffFrames);
	if (diffFrames.channels() > 1)
		cvtColor(diffFrames, diffFrames, COLOR_BGR2GRAY);
	//Checks if the frames are different enough
	double amountDiffPixels = static_cast<double>(countNonZero(diffFrames > 0));
	double amountPixelsNeeded = frame.size().area() * cameraMember.cameraThreshold;
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

void Camera::InitializeChannelAndStub() {
	auto channel = grpc::CreateChannel(cameraMember.cameraIP + ":" + cameraMember.cameraPort, grpc::InsecureChannelCredentials());
	stub = framevideoPackage::FrameVideoService::NewStub(channel);
}

void Camera::sendToServer(VideoCapture cap, Mat frame, string timeStamp) {
	// Convert the image to bytes
	std::vector<uchar> imageData;
	cv::imencode(".png", frame, imageData);
	// Setup request
	framevideoPackage::frame request;
	framevideoPackage::frameResponse result;
	request.set_width(frame.cols);
	request.set_height(frame.rows);
	request.set_channels(frame.channels());
	request.set_imagedata(imageData.data(), imageData.size());
	request.set_timestampvideo(to_string(cap.get(CAP_PROP_POS_MSEC)));
	request.set_timestamp(timeStamp);
	
	//Sending to the server
	grpc::ClientContext context;
	grpc::Status status = stub->getFrame(&context, request, &result);

	if (status.ok())
	{
		//Output result;
		std::cout << "I got:" << std::endl;
		std::cout << "status: " << result.success() << std::endl;
		std::cout << "message: " << result.message() << std::endl;
	}
	else cout << "----------- The connection to the server failed -----------" << endl;
	return;
}
