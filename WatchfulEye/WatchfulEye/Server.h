#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include "Queue.h"
#include "Yolo.h"
#else
#include "Queue.h"
#include "Yolo.h"
#include <unistd.h>
#include <ncurses.h>
#endif

class Server {
public:
	void detections();
};
