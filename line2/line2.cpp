#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;


int rho = 1;
int theta = 1;
int thresh = 140;
int minLineLength=368;
int maxLineGap=70;

int main()
{
	Mat src, dst, color_dst;
	namedWindow("Control", CV_WINDOW_AUTOSIZE);

	createTrackbar("rho", "Control", &rho, 50);	
	createTrackbar("theta", "Control", &theta, 50);
	createTrackbar("thresh", "Control", &thresh, 300);	
	createTrackbar("minLineLength", "Control", &minLineLength, 600);
	createTrackbar("maxLineGap", "Control", &maxLineGap, 100);	


	while (true)
	{
		src = imread("../../BlockVideo/sun1.jpg");
        if( src.empty() )
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }

        resize(src, src, Size(640, 480));

        Canny( src, dst, 50, 200, 3 );
        cvtColor( dst, color_dst, COLOR_GRAY2BGR );
        vector<Vec4i> lines;
        HoughLinesP( dst, lines, rho, CV_PI/180 * theta, thresh, minLineLength, maxLineGap );
        for( size_t i = 0; i < lines.size(); i++ )
        {
            line( color_dst, Point(lines[i][0], lines[i][1]),
            Point( lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
        }
        namedWindow( "Source", 1 );
        imshow( "Source", src );
        namedWindow( "Detected Lines", 1 );
        imshow( "Detected Lines", color_dst );

		char key = (char)waitKey(100);
		if (key == 27)
			break;
	}

	return 0;
}

