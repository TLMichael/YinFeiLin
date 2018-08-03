#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;

int rho = 1;
int theta = 1;
int thresh = 100;
int minLineLength=80;
int maxLineGap=70;

vector<Vec4i> angle_filter(vector<Vec4i> lines)
{
    int distance = 50;
    vector<Vec4i> newlines;
    for(size_t i = 0; i < lines.size(); i++)
    {
        int dx = lines[i][0] - lines[i][2];
        dx = abs(dx);
        int dy = lines[i][1] - lines[i][3];
        dy = abs(dy);
        if(dx < distance || dy < distance)
            newlines.push_back(lines[i]);
    }
    return newlines;
}

void inverse(Mat &frame)
{
        for (int row = 0; row < frame.rows; row++)
        {
            for (int col = 0; col < frame.cols; col++)
            {    
                frame.at<Vec3b>(row, col)[0] = 255 - frame.at<Vec3b>(row, col)[0];
                frame.at<Vec3b>(row, col)[1] = 255 - frame.at<Vec3b>(row, col)[1];
                frame.at<Vec3b>(row, col)[2] = 255 - frame.at<Vec3b>(row, col)[2];
            }
        }
}

int main(int argc, char** argv)
{
    // VideoWriter writer;
    // writer.open("../../BlockVideo/sun2Line.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(1280, 480)); 

    VideoCapture capture;
    Mat src, dst, color_dst;
    capture.open( "../../BlockVideo/sun1.mp4" );
    if ( ! capture.isOpened() ) 
        { printf("--(!)Error opening video capture\n"); return -1; }
    
    while ( capture.read(src) )
    {
        if( src.empty() )
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }
        src = 255 - src;
        resize(src, src, Size(640, 480));

        Canny( src, dst, 50, 200, 3 );
        cvtColor( dst, color_dst, COLOR_GRAY2BGR );
        vector<Vec4i> lines;
        HoughLinesP( dst, lines, rho, CV_PI/180 * theta, thresh, minLineLength, maxLineGap );
        lines = angle_filter(lines);
        for( size_t i = 0; i < lines.size(); i++ )
        {
            line( color_dst, Point(lines[i][0], lines[i][1]),
            Point( lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
        }
        // namedWindow( "Source", 1 );
        // imshow( "Source", src );
        // namedWindow( "Detected Lines", 1 );
        // imshow( "Detected Lines", color_dst );

        Mat comb;
        hconcat(src, color_dst, comb);
        imshow("comb", comb);
        // writer.write(comb);

        char c = waitKey(10);
        if( c == 27 ) { break; } // escape
        else if(c == 's')
        {
            imwrite("../../BlockVideo/sun1.jpg", src);
        }
    }

    return 0;
}