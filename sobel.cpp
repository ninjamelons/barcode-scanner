#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#include <vector>

using namespace cv;
using namespace std;

using Clock=std::chrono::high_resolution_clock;

void ShowManyImages(string title, int nArgs, ...);
Mat ProcessMatImage(cv::Mat &img);

int main(int argc, char* argv[])
{
    std::vector<cv::Mat> imgs = {};

    cv::Mat img1 = cv::imread(argv[1]);
    cv::Mat img2 = cv::imread(argv[2]);
    cv::Mat img3 = cv::imread(argv[3]);
    cv::Mat img4 = cv::imread(argv[4]);

    imgs.push_back(img1);
    imgs.push_back(img2);
    imgs.push_back(img3);
    imgs.push_back(img4);

    int counter = 0;
    for (cv::Mat &img : imgs)
    {
        auto t1 = Clock::now();

        /* Process mat under chrono timed */
        cv::Mat raw;
        raw = ProcessMatImage(img);

        auto t2 = Clock::now();
        std::cout << "Delta t2-t1: " 
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()
                  << " nanoseconds" << std::endl;

        counter++;

        cv::namedWindow(to_string(counter), WINDOW_NORMAL);
        cv::imshow(to_string(counter), img);

        cv::namedWindow("raw" + to_string(counter), WINDOW_NORMAL);
        cv::imshow("raw" + to_string(counter), raw);
    }

    cv::waitKey(0);

    return 0;
}

bool sortContourArea (vector<Point> i, vector<Point> j) { return (contourArea(i) > contourArea(j)); }

Mat ProcessMatImage(cv::Mat &img) {
    //RawImg
    cv::Mat raw = img;
    // Convert RGB Mat to GRAY
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    // Scale down
    double scaleFactor = 0.3;
    cv::resize(img, img, cv::Size(), scaleFactor, scaleFactor);

    // Calculate image gradient
    const int ddepth = CV_32F;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    // Blur for cleaner image
    cv::GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);

    // nth image derivative w/ Sobel operator
    //cv::Sobel(img, grad_x, ddepth, 1, 0, -1);
    //cv::Sobel(img, grad_y, ddepth, 0, 1, -1);

    // 1st image derivative w/ Scharr op
    cv::Scharr(img, grad_x, ddepth, 1, 0);
    cv::Scharr(img, grad_y, ddepth, 0, 1);

    // converting back to CV_8U
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    // Display gradiented img
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, img);

    // Blur to prepare for threshold
    cv::blur(img, img, Size(15,15));

    // Threshold gradient image
    cv::threshold(img, img, 115, 255, THRESH_BINARY);

    // Fill larger gaps between lines
    Mat kernel;
    kernel = cv::getStructuringElement(MORPH_RECT, Size(21, 7));

    cv::morphologyEx(img, img, MORPH_CLOSE, kernel);

    // Remove noisy artifacting
    cv::erode(img, img, kernel, Point(-1,-1), 2, BORDER_CONSTANT, morphologyDefaultBorderValue());
    cv::dilate(img, img, kernel, Point(-1,-1), 5, BORDER_CONSTANT, morphologyDefaultBorderValue());

    // Find and Create largest bounding box (contours)
    cv::Mat ogScale;
    cv::resize(img, ogScale, raw.size(), 0, 0);

    vector<vector<Point>> imgContours;
    vector<Vec4i> hierarchy;
    cv::findContours( ogScale, imgContours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );

    std::sort(imgContours.begin(), imgContours.end(), sortContourArea);

    Point2f vtx[4];
    RotatedRect rect = cv::minAreaRect(imgContours[0]);
    rect.points(vtx);

    for (int i = 0; i < 4; i++)
    {
        line(raw, vtx[i], vtx[(i+1)%4], Scalar(0,255,0), 10, LINE_AA);
    }

    return raw;
}



void ShowManyImages(string title, int nArgs, ...)
{
    int size;
    int i;
    int m, n;
    int x, y;

    // w - Maximum number of images in a row
    // h - Maximum number of images in a column
    int w, h;

    // scale - How much we have to resize the image
    float scale;
    int max;

    // If the number of arguments is lesser than 0 or greater than 12
    // return without displaying
    if(nArgs <= 0) {
        printf("Number of arguments too small....\n");
        return;
    }
    else if(nArgs > 14) {
        printf("Number of arguments too large, can only handle maximally 12 images at a time ...\n");
        return;
    }
    // Determine the size of the image,
    // and the number of rows/cols
    // from number of arguments
    else if (nArgs == 1) {
        w = h = 1;
        size = 300;
    }
    else if (nArgs == 2) {
        w = 2; h = 1;
        size = 300;
    }
    else if (nArgs == 3 || nArgs == 4) {
        w = 2; h = 2;
        size = 300;
    }
    else if (nArgs == 5 || nArgs == 6) {
        w = 3; h = 2;
        size = 200;
    }
    else if (nArgs == 7 || nArgs == 8) {
        w = 4; h = 2;
        size = 200;
    }
    else {
        w = 4; h = 3;
        size = 150;
    }

    // Create a new 3 channel image
    Mat DispImage = Mat::zeros(Size(100 + size*w, 60 + size*h), CV_8UC3);

    // Used to get the arguments passed
    va_list args;
    va_start(args, nArgs);

    // Loop for nArgs number of arguments
    for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {
        // Get the Pointer to the IplImage
        Mat img = va_arg(args, Mat);

        // Check whether it is NULL or not
        // If it is NULL, release the image, and return
        if(img.empty()) {
            printf("Invalid arguments");
            return;
        }

        // Find the width and height of the image
        x = img.cols;
        y = img.rows;

        // Find whether height or width is greater in order to resize the image
        max = (x > y)? x: y;

        // Find the scaling factor to resize the image
        scale = (float) ( (float) max / size );

        // Used to Align the images
        if( i % w == 0 && m!= 20) {
            m = 20;
            n+= 20 + size;
        }

        // Set the image ROI to display the current image
        // Resize the input image and copy the it to the Single Big Image
        Rect ROI(m, n, (int)( x/scale ), (int)( y/scale ));
        Mat temp;
        resize(img,temp, Size(ROI.width, ROI.height));
        temp.copyTo(DispImage(ROI));
    }

    // Create a new window, and show the Single Big Image
    namedWindow( title, 1 );
    imshow( title, DispImage);
    waitKey();

    // End the number of arguments
    va_end(args);
}