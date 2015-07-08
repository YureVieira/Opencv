#include <iostream>
#include <sstream>
#include <math.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#define _RASPI_
//#define ERODE
#define BLOB_AREA_MIN 1000
#define BLOB_AREA_MAX 2000

#ifdef _RASPI_
#include <wiringSerial.h>
#endif // _RASPI_

using namespace cv;
using namespace std;

int main()
{
    VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened())
    {
        cerr<<"Error opening the camera"<<endl;
        return -1;
    }

    cap.set(CV_CAP_PROP_FRAME_HEIGHT,320);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,240);

    Mat frame;
    for(;;)
    {
        cap >> frame;
        imshow( "window", frame );

        int key = waitKey(10);
        if(key==(int)'q')break;     ///Sair com letra q

    }
    destroyAllWindows();
    return 0;
}
