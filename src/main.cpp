#include <iostream>
#include "utils.h"

using namespace std;
using namespace cv;

bool show_visualization = true;
bool sent_serial = false;
bool save_video = true;
bool sent_frame = true;

// const char *IP = "192.168.43.41";

int main()
{
    // SocketMatTransmissionClient socketMat;
    // int socketFlag = -1; 

    string dev = "/dev/ttyUSB0";
    Serial serial((char *)dev.data());

    Detector detector("cfg/yolov3-head.cfg", "cfg/yolov3-head.weights");
    Template tracker;

    VideoCapture capture;
    VideoSaver saver;
    // capture.open( "http://192.168.43.108:8080/video" );
    capture.open( 0 );
    if(!capture.isOpened())
    {
        cout << " --(!) Fail to open camera -- Exit!" << endl;
        exit(0);
    }

    TickMeter meter;
    Mat frame;
    Mat frameShow;
    int status = 0;     // 0：没有目标，1：找到目标进行跟踪
    Rect location;

    while( capture.read(frame) )
    {
        if(frame.empty())
        {
            cout << " --(!) No captured frame -- Break!" << endl;
            break;
        }
        resize(frame, frame, Size(640, 480));
        cv::putText(frame, GetCurrentTime2(), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
        frameShow = frame.clone();

        meter.start();
        if(status == 0)
        {
            vector<bbox_t> results = detector.detect(frame, 0.05);
            vector<Rect> persons = person_filter(results);
            if(persons.size() > 0)
            {
                cout << "[debug] " << meter.getCounter() << ": " << "persons: " << persons.size() << endl;
                for(size_t i = 0; i < persons.size(); i++)
                    rectangle( frameShow, persons[i], Scalar(20, 20, 20), 1, 8, 0);
                location = max_filter(persons);
                rectangle(frameShow, location, Scalar(0, 128, 255), 2);

                tracker.initTracking(frame, location);
                status = 1;
                cout << "[debug] " << meter.getCounter() << ": " << "开始跟踪" << endl;
            }
        }
        else if(status == 1)
        {
            if( meter.getCounter() % 3 != 0 )
            {
                location = tracker.track(frame);
                limitRect(location, frame.size());
                if(location.area() == 0)
                    status = 0;
            }
            else
            {
                vector<bbox_t> results = detector.detect(frame, 0.05);
                vector<Rect> persons = person_filter(results);
                if(persons.size() == 0)
                    status = 0;
                else
                {
                    location = max_filter(persons);
                    tracker.initTracking(frame, location);
                }
            }
            if(status == 1)
            {
                cout << "[loc: " << location.x << " " << location.y  << " " << location.width << " " << location.height << "]\n";
                rectangle(frameShow, location, Scalar(0, 128, 255), 2);
            }
        }
        

        if(sent_serial)
        {
            if(status == 1)
            {
                short int px = location.x + location.width / 2;
                short int py = location.y + location.height / 2;
                px -= frame.cols / 2;
                py -= frame.rows / 2;
                short int distance = 0;
                char buff[9];
                buff[0] = 0xFF;
                buff[1] = 0xFE;
                buff[2] = 0x01;
                buff[3] = (px >> 8) & 0xff;
                buff[4] = px & 0xff;
                buff[5] = (py >> 8) & 0xff;
                buff[6] = py & 0xff;
                buff[7] = (distance >> 8) & 0xff;
                buff[8] = distance & 0xff;
                buff[9] = (buff[3] + buff[4] + buff[5] + buff[6] + buff[7] + buff[8]) % 0xff;
                
                serial.writeData(buff, 9);
                cout << "[debug] " << px << ' ' << py << ' ' << location.height << endl;
            }
            else
            {
                char buff[9];
                buff[0] = 0xFF;
                buff[1] = 0xFE;
                buff[2] = 0x00;
                buff[3] = 0x00;
                buff[4] = 0x00;
                buff[5] = 0x00;
                buff[6] = 0x00;
                buff[7] = 0x00;
                buff[8] = 0x00;
                buff[9] = 0x00;
                serial.writeData(buff, 9);
            }
        }
        
        if(show_visualization)
        {
            imshow("YinFeiLin", frameShow);
            char key = waitKey(1);
            if (key == 27 || key == 'q' || key == 'Q')
                break;
        }

        if(save_video)
        {
            saver.write(frameShow);
        }

        if(sent_frame)
        {
            // if(socketFlag < 0 && meter.getCounter() % 50 == 0)
            // {
            //     socketFlag = socketMat.socketConnect(IP, 6666); 
            // }
            // if(socketFlag >= 0)
            //     socketMat.transmit(frameShow);
            send_mjpeg(frameShow, 12345);
        }

        meter.stop();
    }

    double fps = double(meter.getCounter()) / meter.getTimeSec();
    cout << "[YinFeiLin] " << "fps: " << fps << endl;

    return 0;
}