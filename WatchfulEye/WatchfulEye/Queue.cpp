#include "Queue.h"
#include "Logging.h"

VideoFrameQueue* VideoFrameQueue::instancePtr = NULL;

VideoFrameQueue& VideoFrameQueue::getInstance()
{
	if (instancePtr == NULL) {
		instancePtr = new VideoFrameQueue();
		return *instancePtr;
	}
	else return *instancePtr;
}

VideoFrameData& VideoFrameQueue::front() {
	std::unique_lock<std::mutex> lock(g_mutex);
	g_cv.wait(lock, [this] { return ready || stopServer; });
	if (stopServer)
		throw std::runtime_error("ESC key was pressed");
	return container.front();
}

void VideoFrameQueue::push(const VideoFrameData& data) {
	if (size() == 5)
	{
		VideoFrameData poppedData = pop();
		std::string info = "Pop from the queueFrames, a frame that was at time: " + poppedData.timestampVideo + " in the video, Because the queue is limited to 5";
		Logging::writeTolog(spdlog::level::info, info);
	}
	std::lock_guard<std::mutex> lock(g_mutex);
	//critical section
	container.push(data);
	std::string info1 = "Push to the frame queue, a frame that was at time: " + data.timestampVideo + " in the video";
	Logging::writeTolog(spdlog::level::info, info1);
	ready = true;
	g_cv.notify_one();

}

VideoFrameData VideoFrameQueue::pop() {
	std::unique_lock<std::mutex> lock(g_mutex);
	g_cv.wait(lock, [this] { return ready || stopServer; });
	//critical section
	if (stopServer)
		throw std::runtime_error("ESC key was pressed");
	VideoFrameData data = container.front();
	container.pop();
	if (container.empty())
		ready = false;
	return data;
}

bool VideoFrameQueue::empty() {
	std::lock_guard<std::mutex> lock(g_mutex);
	return container.empty();
}

size_t VideoFrameQueue::size() {
	std::lock_guard<std::mutex> lock(g_mutex);
	return container.size();
}
