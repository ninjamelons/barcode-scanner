#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdarg.h>

#include "threshold.h"
#include "decoder.h"
#include "code39.h"
#include "ean13.h"

using namespace cv;

cv::Mat img_close, img_med, img_far,
        img_close_thresh, img_med_thresh, img_far_thresh;

BScanner::Threshold thresh;
int dim_slider, threshold_slider, blur_slider;

void processImg(const Mat& img, Mat& imgOut);
void decodeImg(const Mat& img);

void On_Trackbar( int, void* )
{
    thresh.SetBlur((int) blur_slider);
    thresh.SetThreshold((int) threshold_slider);

    // Process imgs here
    processImg(img_close, img_close_thresh);
    processImg(img_med, img_med_thresh);
    processImg(img_far, img_far_thresh);

    // Decode img here
    decodeImg(img_close_thresh);
    decodeImg(img_med_thresh);
    decodeImg(img_far_thresh);
    
    cv::imshow("Close Proc", img_close_thresh);
    cv::imshow("Med Proc", img_med_thresh);
    cv::imshow("Far Proc", img_far_thresh);
}

int main(int argc, char* argv[])
{
    // Processed Windows
    cv::namedWindow("Close Proc", cv::WINDOW_NORMAL);
    cv::namedWindow("Med Proc", cv::WINDOW_NORMAL);
    cv::namedWindow("Far Proc", cv::WINDOW_NORMAL);

    // Load barcodes where distance to camera is ...
    img_close = cv::imread(argv[1]);
    img_med = cv::imread(argv[2]);
    img_far = cv::imread(argv[3]);

    //Process image (Minor thresholding and blur for easier/cleaner decode)
    int dim_max, threshold_max, blur_max;

    dim_max = 1924;
    threshold_max = 255;
    blur_max = 31;

    dim_slider = 300;
    threshold_slider = 110;
    blur_slider = 1;

    thresh.SetBlur((int) blur_slider);
    thresh.SetThreshold((int) threshold_slider);

    cv::namedWindow("Trackbars", cv::WINDOW_NORMAL);
    cv::createTrackbar( "Dimension", "Trackbars", &dim_slider, dim_max, On_Trackbar );
    cv::createTrackbar( "Threshold", "Trackbars", &threshold_slider, threshold_max, On_Trackbar );
    cv::createTrackbar( "Blur", "Trackbars", &blur_slider, blur_max, On_Trackbar );

    On_Trackbar( 0,0 );

    cv::waitKey(0);

    return 0;
}

void processImg(const Mat& img, Mat& imgOut)
{
    // Scale down - Super buggy with trackbar slider
    float aspectTransform = img.cols / dim_slider;
    cv::resize(img, imgOut, cv::Size(), 1/aspectTransform, 1/aspectTransform);

    // Convert RGB Mat to GRAY
    cv::cvtColor(imgOut, imgOut, cv::COLOR_BGR2GRAY);

    // Threshold gradient image
    cv::threshold(imgOut, imgOut, thresh.GetThreshold(), 255, THRESH_BINARY);
}

void decodeImg(const cv::Mat& img)
{
    BScanner::Decoder* decoder = new BScanner::Ean13();
    BScanner::Result res = decoder->decode(img);

    // Print decoded value & coordinates
    std::cout << res.symbology <<": "<< res.value << "; ";
    if(res.coords.size() > 0)
    {
        std::cout << "Coords: ["<<
            res.coords[0] <<','<< res.coords[1] <<','<< res.coords[2] <<','<< res.coords[2] << std::endl;
    }
}