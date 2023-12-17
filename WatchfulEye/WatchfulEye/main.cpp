#include "WatchfulEye.h"
#define VIDEO_PATH R"(C:\projectSources\vid_Trim_Trim.mp4)"
int main()
{
    Yolo yolo;
    yolo.csv.start();
    WatchfulEye watch;
    VideoCapture cap(VIDEO_PATH);
    if (!cap.isOpened()) {
        throw std::invalid_argument("Error opening video stream or file");
    }
    Mat frame;
    Mat prevFrame;
    cap >> frame;
    if (!frame.empty()) {
        watch.pushToTheQueue(cap, frame);
        watch.detections();
    }
    prevFrame = frame;
    char c = 0;
    while (!frame.empty() && (c != 27)) {
        Mat frame;
        cap >> frame;
        if (frame.empty())
            break;
        if (watch.cmpFrames(prevFrame, frame)) {
            watch.pushToTheQueue(cap, frame);
            watch.detections();
        }
        prevFrame = frame;
        c = (char)waitKey(25);
    }
    yolo.csv.close();
    cap.release();
    destroyAllWindows();
    return 0;
}