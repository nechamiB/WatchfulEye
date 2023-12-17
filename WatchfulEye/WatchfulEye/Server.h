#include <mutex>
#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>
#include "myproto/frameVideo.grpc.pb.h"
#include "Queue.h"
#include  "Yolo.h"
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <ncurses.h>
#endif

class Server {
private:
	static ConfigurationServer configurationServer;
	static serverMembers serverMember;
public:
	void popFromDataStruct();
	void detections(cv::Mat frame, std::string timestamp);
	void pushToTheDataStruct(const cv::Mat& frame, const std::string& timestampVideo, const std::string& timeStamp);
	void RunServer();
	void exitByESC();
	cv::Mat convertToImage(const framevideoPackage::frame& frameMsg);
};

class classFrameService final : public framevideoPackage::FrameVideoService::Service {
private:
	std::mutex m_mutex;
public:
	virtual ::grpc::Status getFrame(::grpc::ServerContext* context, const ::framevideoPackage::frame* request, ::framevideoPackage::frameResponse* response) {
		std::lock_guard<std::mutex> lock(m_mutex);
		Server s;
		s.pushToTheDataStruct(s.convertToImage(*request), request->timestampvideo(), request->timestamp());
		response->set_success(true);
		response->set_message("I got it thanks!");
		return grpc::Status::OK;
	}
};


