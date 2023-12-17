#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include "myproto/frameVideo.grpc.pb.h"
#include <chrono>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#include "þþConfigurationCamera.h"
#else
#include "þþConfigurationCamera.h"
#include <unistd.h>
#include <ncurses.h>
#endif
#include "opencv2/opencv.hpp"


class Camera {
private:
	static ConfigurationCamera configurationCamera;
	static cameraMembers cameraMember;
	std::unique_ptr<framevideoPackage::FrameVideoService::Stub> stub;
public:
	void start();
	bool cmpFrames(cv::Mat prevFrame, cv::Mat frame);
	std::string calcTime();
	void InitializeChannelAndStub();
	void sendToServer(cv::VideoCapture cap, cv::Mat frame, std::string timeStamp);
};
