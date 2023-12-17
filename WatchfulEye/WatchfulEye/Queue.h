#pragma once
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>

typedef struct videoFrameData {
	std::string timestamp;
	std::string timestampVideo;
	cv::Mat frame;
} VideoFrameData;

class VideoFrameQueue {
private:
	std::mutex g_mutex;
	std::condition_variable g_cv;
	bool ready = false;
	std::atomic<bool> stopServer;
	std::queue<VideoFrameData> container;
	static VideoFrameQueue* instancePtr;
	VideoFrameQueue() {}

public:
	// deleting copy constructor
	VideoFrameQueue(const VideoFrameQueue& obj) = delete;
	static VideoFrameQueue& getInstance();
	VideoFrameData& front();
	void push(const VideoFrameData& data);
	VideoFrameData pop();
	bool empty();
	size_t size();
	bool getReady() { return ready; }
	void setStopServer(bool value) { stopServer = value; }
};
