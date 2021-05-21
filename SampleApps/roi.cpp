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
    cv::Mat img = cv::imread(argv[1]);

    cv::Mat raw, firstIter;
    firstIter = ProcessMatImage(img);

    cv::namedWindow("Filter", WINDOW_NORMAL);
    cv::imshow("Filter", img);

    cv::namedWindow("Return", WINDOW_NORMAL);
    cv::imshow("Return", firstIter);
    cv::waitKey(0);

    return 0;
}

bool sortContourArea (vector<Point> i, vector<Point> j) { return (contourArea(i) > contourArea(j)); }

Mat ProcessMatImage(cv::Mat &img) {
    //RawImg
    cv::Mat raw = img;

    // Convert RGB Mat to GRAY
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    // Calculate image gradient
    const int ddepth = CV_32F;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    // Blur for cleaner image
    cv::GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);

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
    cv::erode(img, img, kernel, Point(-1,-1), 3.5, BORDER_CONSTANT, morphologyDefaultBorderValue());

    // Enlarge main focus
    cv::dilate(img, img, kernel, Point(-1,-1), 18, BORDER_CONSTANT, morphologyDefaultBorderValue());

    vector<vector<Point>> imgContours;
    vector<Vec4i> hierarchy;
    cv::findContours( img, imgContours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );

    std::sort(imgContours.begin(), imgContours.end(), sortContourArea);

    Mat M, rotated, cropped;
    Point2f vtx[4];
    if(!imgContours.empty()) {
        RotatedRect rect = cv::minAreaRect(imgContours[0]);

        cv::Mat3b roi3b(raw(rect.boundingRect()));

        GaussianBlur(roi3b, roi3b, Size(), 10);

        cv::namedWindow("Rect", WINDOW_NORMAL);
        cv::imshow("Rect", raw);

        // get angle and size from the bounding box
        float angle = rect.angle;
        Size rect_size = rect.size;

        if (angle < -45.) {
            angle += 90.0;
            cv::swap(rect_size.width, rect_size.height);
        }

        // get the rotation matrix
        M = getRotationMatrix2D(rect.center, angle, 1.0);
        // perform the affine transformation
        warpAffine(raw, rotated, M, raw.size(), INTER_CUBIC);
        // crop the resulting image
        getRectSubPix(rotated, rect_size, rect.center, cropped);

        rect.points(vtx);
    
        for (int i = 0; i < 4; i++)
        {
            line(raw, vtx[i], vtx[(i+1)%4], Scalar(0,255,0), 10, LINE_AA);
        }
    }

    return cropped;
}