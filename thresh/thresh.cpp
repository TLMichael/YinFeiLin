#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
using namespace std;
using namespace cv;

int main( int argc, const char** argv )
{
    VideoCapture capture;
    Mat frame;
    capture.open( "../../BlockVideo/sun1.mp4" );
    if ( ! capture.isOpened() ) 
        { printf("--(!)Error opening video capture\n"); return -1; }
    
    while ( capture.read(frame) )
    {
        if( frame.empty() )
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }


        imshow("Circle", frame);
        char c = waitKey(0);
        if( c == 27 ) { break; } // escape
    }

    waitKey(0);
    return 0;
}
