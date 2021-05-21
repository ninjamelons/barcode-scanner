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
    cv::VideoCapture capture(0); // Test random numbers (e.g. 0-3) devices swap around whenever they feel like it
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
    cv::namedWindow("Image", cv::WINDOW_NORMAL);
    cv::namedWindow("Image ROI", cv::WINDOW_NORMAL);

    // Setup trackbar vars
    const int one_threshold_max = 255;
    const int one_blur_max = 31;

    int one_threshold_slider = 115;
    int one_blur_slider = 15;

    BScanner::Threshold thresh1;

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
        cv::createTrackbar( OneThresholdTrackbar, "Image", &one_threshold_slider, one_threshold_max );
        char OneBlurTrackbar[50];
        sprintf( OneBlurTrackbar, "Blur x %d", one_blur_max );
        cv::createTrackbar( OneBlurTrackbar, "Image", &one_blur_slider, one_blur_max );

        thresh1.SetBlur((int) one_blur_slider);
        thresh1.SetThreshold((int) one_threshold_slider);

        std::vector<std::tuple<cv::Mat, cv::RotatedRect>> passes; 
    
        // Process Frame
        imgPrc.ThresholdPass( false, frame, passes, thresh1 );

        // Draw Rect on frame
        cv::Point2f vtx[4];
        if(!passes.empty()) {
            std::get<1>(passes[0]).points(vtx);
            for (int i = 0; i < 4; i++)
            {
                line(frame, vtx[i], vtx[(i+1)%4], cv::Scalar(0,255,0), 10, cv::LINE_AA);
            }
            cv::imshow("Image ROI", std::get<0>(passes[0]));
        }
        
        cv::imshow("Image", frame);

        int key = cv::waitKey(1);
        if (key == 27/*ESC*/)
        {
            break;
        }
    }
    return 0;
}