#include <opencv2/core.hpp> // core / cv scope
#include <opencv2/imgcodecs.hpp> // imread()
#include <opencv2/videoio.hpp> // videocapture
#include <opencv2/highgui.hpp> // waitkey, namedwindow, createtrackbar, imshow

#include <iostream>
#include <vector>

#include "process.h"
#include "decoder.h"

int main(int, char**)
{
    cv::Mat frame;
    std::cout << "Opening camera..." << std::endl;
    cv::VideoCapture capture(1); // Test random numbers (e.g. 0-3) devices swap around whenever they feel like it
    BScanner::ImgProcess imgPrc;

    // Set targeted FPS (image reads at FPS, but processing may take longer)
    int FPS = 30;
    capture.set(cv::CAP_PROP_FPS, (double)FPS); // May affect processing/decoding poorly due to forced timings

    if (!capture.isOpened())
    {
        std::cerr << "ERROR: Can't initialize camera capture" << std::endl;
        return 1;
    }

    // Create Windows
    cv::namedWindow("Image", cv::WINDOW_NORMAL);
    cv::namedWindow("Image ROI", cv::WINDOW_NORMAL);

    // Setup trackbar vars
    const int one_threshold_max = 255;
    const int one_blur_max = 31;
    const int one_kernelx_max = 31;
    const int one_kernely_max = 31;
    const int one_erode_max = 60; // divide by 2 - 30 max increments of 0.5
    const int one_dilate_max = 60; // divide by 2 - 30 max increments of 0.5

    int one_threshold_slider = 130;
    int one_blur_slider, one_kernelx_slider, one_kernely_slider;
    int one_erode_slider, one_dilate_slider;
    one_blur_slider = one_kernelx_slider = one_kernely_slider = 13;
    one_erode_slider = one_dilate_slider = 8;

    BScanner::Threshold thresh1;

    char OneThresholdTrackbar[50];
    sprintf( OneThresholdTrackbar, "Threshold x %d", one_threshold_max );
    cv::createTrackbar( OneThresholdTrackbar, "Image", &one_threshold_slider, one_threshold_max );
    char OneBlurTrackbar[50];
    sprintf( OneBlurTrackbar, "Blur x %d", one_blur_max );
    cv::createTrackbar( OneBlurTrackbar, "Image", &one_blur_slider, one_blur_max );
    char OneKernelXTrackbar[50];
    sprintf( OneKernelXTrackbar, "KernelX x %d", one_kernelx_max );
    cv::createTrackbar( OneKernelXTrackbar, "Image", &one_kernelx_slider, one_kernelx_max );
    char OneKernelYTrackbar[50];
    sprintf( OneKernelYTrackbar, "KernelY x %d", one_kernely_max );
    cv::createTrackbar( OneKernelYTrackbar, "Image", &one_kernely_slider, one_kernely_max );
    char OneErodeTrackbar[50];
    sprintf( OneErodeTrackbar, "Erode x %d", one_erode_max );
    cv::createTrackbar( OneErodeTrackbar, "Image", &one_erode_slider, one_erode_max );
    char OneDilateTrackbar[50];
    sprintf( OneDilateTrackbar, "Dilatex %d", one_dilate_max );
    cv::createTrackbar( OneDilateTrackbar, "Image", &one_dilate_slider, one_dilate_max );

    for (;;)
    {
        capture >> frame; // read the next frame from camera
        if (frame.empty())
        {
            std::cerr << "ERROR: Can't grab camera frame." << std::endl;
            break;
        }

        thresh1.SetBlur( one_blur_slider );
        thresh1.SetThreshold( one_threshold_slider );
        thresh1.SetKernelXY( one_kernelx_slider, one_kernely_slider );
        thresh1.SetErode( one_erode_slider );
        thresh1.SetDilate( one_dilate_slider );
    
        // Process Frame
        cv::RotatedRect rect;
        rect = imgPrc.FarThresholdPass( frame, thresh1 );

        cv::Mat patch;
        try
        {
            patch = frame(rect.boundingRect());
        }
        catch (cv::Exception exc)
        {
            patch = frame;
        }

        // Draw Rect on frame
        cv::Point2f vtx[4];
        rect.points(vtx);
        for (int i = 0; i < 4; i++)
        {
            line(frame, vtx[i], vtx[(i+1)%4], cv::Scalar(0,255,0), 10, cv::LINE_AA);
        }
        cv::imshow("Image ROI", patch);
        
        cv::imshow("Image", frame);

        int key = cv::waitKey(1);
        if (key == 27/*ESC*/)
        {
            break;
        }
    }
    return 0;
}