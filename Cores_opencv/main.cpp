#include <iostream>
#include <opencv.hpp>
#include <math.h>
#define FAIXA_H 5
#define FAIXA_S 15
#define FAIXA_V 5

using namespace std;
using namespace cv;

Mat image_HSV;
int _x,_y;
Vec3b color;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    //http://opencv-srf.blogspot.com.br/2011/11/mouse-events.html
    static bool flag_Release = false;
    if  ( event == EVENT_LBUTTONDOWN )
    {
        flag_Release = true;
        _x=x;
        _y=y;
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if  ( event == EVENT_LBUTTONUP )
    {
        flag_Release = false;
        color = image_HSV.at<Vec3b>(Point(_x,_y));
        //Mat M(200,200,CV_8UC3,Scalar((int)color[0],255,100));
        //cvtColor(M,M,CV_HSV2BGR);
        //imshow("Color",M);
        cout << "color_HSV: " << color << endl;
    }
    else if ( event == EVENT_MOUSEMOVE && flag_Release)
    {
        _x=x;
        _y=y;
    }
}
void show_xy_color(Mat img,int x,int y)
{
        Vec3b color = img.at<Vec3b>(Point(x,y));
        Mat M(200,200,CV_8UC3,Scalar((int)color[0],255,100));
        cvtColor(M,M,CV_HSV2BGR);
        imshow("Color",M);
        cout << "color_HSV: " << color << endl;
}
int main()
{
    VideoCapture cap(0);
    if(!cap.isOpened()) // check if we succeeded
        return 1;

    cap.set(CV_CAP_PROP_FRAME_HEIGHT,640);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,480);

    namedWindow("window",1);
    namedWindow("H",1);
    //namedWindow("Color",1);
    //set the callback function for any mouse event
    setMouseCallback("window", CallBackFunc, NULL);

    Mat frame,aux_hsv,result;
    vector<Mat> HSV_chanells;

    for(;;)
    {
        cap >> frame;
        GaussianBlur(frame, frame, Size(5, 5), 2, 2 );//aplique um filtro
        cvtColor(frame,aux_hsv,CV_BGR2HSV);
        aux_hsv.copyTo(image_HSV);

        //Separa canais para verificar somente Matiz.
        split(aux_hsv,HSV_chanells);

        threshold(HSV_chanells[0],HSV_chanells[0],min(255,(int)color[0]+FAIXA_H),255,THRESH_TOZERO_INV);
        threshold(HSV_chanells[0],HSV_chanells[0],max(0,(int)color[0]-FAIXA_H),255,THRESH_TOZERO);
        threshold(HSV_chanells[0],HSV_chanells[0],1,255,THRESH_BINARY);

        threshold(HSV_chanells[1],HSV_chanells[1],min(255,(int)color[1]+FAIXA_S),255,THRESH_TOZERO_INV);
        threshold(HSV_chanells[1],HSV_chanells[1],max(0,(int)color[1]-FAIXA_S),255,THRESH_TOZERO);
        threshold(HSV_chanells[1],HSV_chanells[1],1,255,THRESH_BINARY);
        //bitwise_not(HSV_chanells[1],HSV_chanells[1]);

        threshold(HSV_chanells[2],HSV_chanells[2],100,255,THRESH_TOZERO_INV);
        //threshold(HSV_chanells[2],HSV_chanells[2],20,255,THRESH_TOZERO);
        threshold(HSV_chanells[2],HSV_chanells[2],1,255,THRESH_BINARY);
        bitwise_not(HSV_chanells[2],HSV_chanells[2]);

        bitwise_and(HSV_chanells[0],HSV_chanells[1],result);
        bitwise_and(result,HSV_chanells[2],result);

        //Junta novamete os canais.
        //merge(HSV_chanells,aux_hsv);
        //cvtColor(aux_hsv,result,CV_HSV2BGR);

        //imshow("debug",HSV_chanells[2]);
        circle(frame,Point(_x,_y),10,Scalar(100,200,230),3,1);
        show_xy_color(image_HSV,_x,_y);

        imshow("H",HSV_chanells[0]);
        imshow("S",HSV_chanells[1]);
        imshow("V",HSV_chanells[2]);
        imshow("result",result);

        imshow( "window", frame );

        if(waitKey(10)==(int)'q')break;
    }

    destroyAllWindows();
    return 0;
}
