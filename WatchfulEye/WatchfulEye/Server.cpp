#include "Server.h"

using namespace std;
using namespace cv;

//Initialize the static variables.are used to get the configurations
ConfigurationServer Server::configurationServer;
serverMembers Server::serverMember = configurationServer.getServerMembers();

VideoFrameQueue& frameQueue = VideoFrameQueue::getInstance();

void Server::popFromDataStruct()
{
	Sqlite sqlite;
	sqlite.createTable(sqlite.path);
	while (!frameQueue.getStopServer()) {
		//pop frame from the queue
		VideoFrameData data;
		try { data = frameQueue.pop(); }
		catch (const std::runtime_error& e) { break; }

		Mat frame = data.frame;
		string timestamp = data.timestamp;
		detections(frame, timestamp);
	}
}

void Server::detections(Mat frame, string timestamp)
{
	Yolo yolo;
	cv::dnn::Net net;
	try {
		net = cv::dnn::readNet(serverMember.windowsYoloPath);
		if (net.empty()) {
			throw std::runtime_error("Error:Yolo- readNet failed.");
		}
	}
	catch (const std::exception& e) {
		Logging::writeTolog(spdlog::level::err, e.what());
	}
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
}

void Server::RunServer()
{
	string server_address(serverMember.backendIP + ":" + serverMember.backendPort);
	grpc::ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	classFrameService my_service;
	builder.RegisterService(&my_service);
	unique_ptr<grpc::Server> server(builder.BuildAndStart());
	exitByESC();
}

void Server::exitByESC()
{
#ifdef __linux__
	initscr();  // Initialize the ncurses library
	keypad(stdscr, TRUE);  // Enable reading of function keys like ESC
	nodelay(stdscr, TRUE);  // Enable non-blocking input
#endif
	int c = 0;
	while (c != 27 /*ESC*/) {
#ifdef __linux__
		c = getch();  // Read a character from the keyboard
#else
		if (_kbhit()) {
			c = _getch();
		}
#endif
		if (c == 27) {
			string infoChar = "The server thread exits because the ESC key was pressed.";
			Logging::writeTolog(spdlog::level::info, infoChar);
			frameQueue.setStopServer(true);
			frameQueue.notifyThread();
			break;
		}
	}
#ifdef __linux__
	endwin();  // Clean up the ncurses library
#endif
}

Mat Server::convertToImage(const framevideoPackage::frame& frameMsg)
{
	// Decode the image data
	std::vector<uchar> imageData(frameMsg.imagedata().begin(), frameMsg.imagedata().end());
	cv::Mat image = cv::imdecode(imageData, cv::IMREAD_COLOR);

	// Ensure that the image channels match the expected channels on the server
	if (image.channels() != frameMsg.channels())
		cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

	// Resize the image if necessary
	if (image.cols != frameMsg.width() || image.rows != frameMsg.height())
		cv::resize(image, image, cv::Size(frameMsg.width(), frameMsg.height()));
	return image;
}

void Server::pushToTheDataStruct(const Mat& frame, const string& timestampVideo, const string& timeStamp)
{
	VideoFrameData currentFrame;
	currentFrame.timestampVideo = timestampVideo;
	currentFrame.timestamp = timeStamp;
	currentFrame.frame = frame;
	frameQueue.push(currentFrame);
}