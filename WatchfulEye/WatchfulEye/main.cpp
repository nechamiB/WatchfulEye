#include "Server.h"
#include "Camera.h"
#include <thread>

using namespace cv;
using namespace std;

int main()
{
	Logging::initLogger();

	// Create instances of Camera and Server
	Camera camera;
	Server server;

	// Create threads and link them to start functions
	std::thread cameraThread(&Camera::start, &camera);
	std::thread serverThread(&Server::detections, &server);

	// Start the threads
	cameraThread.join();
	serverThread.join();

	return 0;
}