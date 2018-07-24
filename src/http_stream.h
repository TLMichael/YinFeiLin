#ifndef HTTP_STREAM_H
#define HTTP_STREAM_H

#include <opencv2/opencv.hpp>
using namespace cv;

void send_mjpeg(Mat img, int port, int timeout = 200, int quality = 30);


#endif // HTTP_STREAM_H