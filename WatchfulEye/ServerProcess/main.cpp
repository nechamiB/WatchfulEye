#include "Server.h"
#include <thread>

int main() {
	Server server;
	//Server server;
	std::thread pushingThread(&Server::RunServer, &server);
	std::thread popingThread(&Server::popFromDataStruct, &server);
	// Start the threads
	pushingThread.join();
	popingThread.join();

	return 0;
}