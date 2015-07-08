#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

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

    Point center = Point(cap.get(CV_CAP_PROP_FRAME_WIDTH)/2,cap.get(CV_CAP_PROP_FRAME_HEIGHT)/2);
    Point beginPoint = Point(0,center.y);
    Point endPoint = Point(center.x*2,center.y);
    int cut_vertical = center.y - 25;
    int threshold_value = 93;
    Mat frame,gray;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    namedWindow("window",1);
    createTrackbar("threshold","window", &threshold_value, 255);
    for(;;)
    {
        cap >> frame;
        cvtColor(frame,gray,CV_BGR2GRAY);
        circle(frame,center,15,Scalar(255,0,0),5);
        line(frame,beginPoint,endPoint,Scalar(0,0,255),5);
        //Algoritimo
        Mat imgCut = Mat(gray, Rect(0,//Imagem cortada
                                     cut_vertical,
                                     cap.get(CV_CAP_PROP_FRAME_WIDTH),
                                     cap.get(CV_CAP_PROP_FRAME_HEIGHT) - cut_vertical*2));

        threshold(imgCut,imgCut,threshold_value,255,CV_THRESH_BINARY);
        bitwise_not(imgCut,imgCut);
        Mat erodeElmt = getStructuringElement(MORPH_RECT, Size(3, 3));
        Mat dilateElmt = getStructuringElement(MORPH_RECT, Size(5, 5));
        erode(imgCut, imgCut, erodeElmt);
        dilate(imgCut, imgCut, dilateElmt);

        findContours(imgCut, contours, hierarchy, CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));
        for (size_t i = 0; i < contours.size(); i++) {
           float area = contourArea(contours[i]);
           if (area > 2000) {
              Moments mu;
              mu = moments(contours[i], false);
              Point2f center_blob(mu.m10 / mu.m00, 240); // point in center (x only)
              circle(frame, Point2f(center_blob.x,(float)center.y), 5, Scalar(0, 255, 0), -10, 8, 0);
           }
        }
        //***************************************************************

        cv::imshow( "window", frame);
        //cv::imshow( "cut", imgCut);

        int key = cv::waitKey(10);
        if(key==(int)'q')break;     ///Sair com letra q

    }
    cv::destroyAllWindows();
    return 0;
}
/******************************************************************************/

