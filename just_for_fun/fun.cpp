#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap;
    cap.open( 0 );
    if(!cap.isOpened())
    {
        cout << "cap is not open" << endl;
        return 0;
    }

    Mat frame, dst, comb;
    while(cap.read(frame))
    {
        if(frame.empty())
        {
            cout << "frame is empty" << endl;
            break;
        }
        dst = frame.clone();
        
        for (int row = 0; row < frame.rows; row++)
        {
            for (int col = 0; col < frame.cols; col++)
            {    
                /* 注意 Mat::at 函数是个模板函数, 需要指明参数类型, 因为这张图是具有红蓝绿三通道的图,
                所以它的参数类型可以传递一个 Vec3b, 这是一个存放 3 个 uchar 数据的 Vec(向量). 这里
                提供了索引重载, [2]表示的是返回第三个通道, 在这里是 Red 通道, 第一个通道(Blue)用[0]返回 */
                dst.at<Vec3b>(row, col)[0] = 255 - frame.at<Vec3b>(row, col)[0];
                dst.at<Vec3b>(row, col)[1] = 255 - frame.at<Vec3b>(row, col)[1];
                dst.at<Vec3b>(row, col)[2] = 255 - frame.at<Vec3b>(row, col)[2];
            }
        }

        hconcat(frame, dst, comb);
        imshow("comb", comb);
        waitKey(10);
    }

    return 0;
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