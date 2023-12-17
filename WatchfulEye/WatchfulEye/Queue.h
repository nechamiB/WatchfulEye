#pragma once
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#ifdef _WIN32
#include "þþConfigurationServer.h"
#else
#include "ConfigurationServer.h"
#endif

typedef struct videoFrameData {
	std::string timestamp;
	std::string timestampVideo;
	cv::Mat frame;
} VideoFrameData;

class VideoFrameQueue {
private:
	static ConfigurationServer configurationServer;
	static int backendQueueSize;
	std::atomic<bool> stopServer;
	std::mutex g_mutex;
	std::condition_variable g_cv;
	bool ready = false;
	std::queue<VideoFrameData> container;
	static VideoFrameQueue* instancePtr;
	VideoFrameQueue();
public:
	VideoFrameQueue(const VideoFrameQueue& obj) = delete;
	static VideoFrameQueue& getInstance();
	VideoFrameData& front();
	void push(const VideoFrameData& data);
	VideoFrameData pop();
	bool empty();
	size_t size();
	bool getReady() { return ready; }
	void setStopServer(bool value) { stopServer = value;}
	bool getStopServer() { return stopServer; }
	void notifyThread(){ g_cv.notify_one(); }
};