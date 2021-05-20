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
    cv::VideoCapture capture(2); // Test random numbers (e.g. 0-3) devices swap around whenever they feel like it
    BScanner::ImgProcess imgPrc;

    // Set targeted FPS (image reads at FPS, but processing may take longer)
    int FPS = 30;
    capture.set(cv::CAP_PROP_FPS, (double)FPS);

    if (!capture.isOpened())
    {
        std::cerr << "ERROR: Can't initialize camera capture" << std::endl;
        return 1;
    }

    // Create Windows
    cv::namedWindow("Frame", cv::WINDOW_NORMAL);
    cv::namedWindow("One", cv::WINDOW_NORMAL);
    cv::namedWindow("Two", cv::WINDOW_NORMAL);

    // Setup trackbar vars
    const int one_threshold_max = 255;
    const int one_blur_max = 31;
    const int two_threshold_max = 255;
    const int two_blur_max = 31;

    int one_threshold_slider = 115;
    int one_blur_slider = 15;
    int two_threshold_slider = 115;
    int two_blur_slider = 15;

    int one_threshold;
    int one_blur;
    int two_threshold;
    int two_blur;

    for (;;)
    {
        capture >> frame; // read the next frame from camera
        if (frame.empty())
        {
            std::cerr << "ERROR: Can't grab camera frame." << std::endl;
            break;
        }

        char OneThresholdTrackbar[50];
        sprintf( OneThresholdTrackbar, "Threshold x %d", one_threshold_max );
        cv::createTrackbar( OneThresholdTrackbar, "One", &one_threshold_slider, one_threshold_max );
        char OneBlurTrackbar[50];
        sprintf( OneBlurTrackbar, "Blur x %d", one_blur_max );
        cv::createTrackbar( OneBlurTrackbar, "One", &one_blur_slider, one_blur_max );

        char TwoThresholdTrackbar[50];
        sprintf( TwoThresholdTrackbar, "Threshold x %d", two_threshold_max );
        cv::createTrackbar( TwoThresholdTrackbar, "Two", &two_threshold_slider, two_threshold_max );
        char TwoBlurTrackbar[50];
        sprintf( TwoBlurTrackbar, "Blur x %d", two_blur_max );
        cv::createTrackbar( TwoBlurTrackbar, "Two", &two_blur_slider, two_blur_max );

        if(one_blur_slider % 2 == 0) {
            one_blur = one_blur_slider + 1;
        } else {
            one_blur = (int) one_blur_slider;
        }
        one_threshold = (int) one_threshold_slider;

        if(two_blur_slider % 2 == 0) {
            two_blur = two_blur_slider + 1;
        } else {
            two_blur = (int) two_blur_slider;
        }
        two_threshold = (int) two_threshold_slider;

        cv::Mat frameClean, frameOne, frameTwo;
        std::vector<cv::Mat> passes; 
        
        // Process Frame
        imgPrc.ThresholdPass( false, frame, passes );
        
        cv::imshow("Frame", frame);
        cv::imshow("One", frame);
        cv::imshow("Two", frame);

        int key = cv::waitKey(1);
        if (key == 27/*ESC*/)
        {
            break;
        }
    }
    return 0;
}