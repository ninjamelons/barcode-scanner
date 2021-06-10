#include <opencv2/core.hpp> // core / cv scope
#include <opencv2/imgcodecs.hpp> // imread()
#include <opencv2/videoio.hpp> // videocapture
#include <opencv2/highgui.hpp> // waitkey, namedwindow, createtrackbar, imshow

#include <iostream>
#include <vector>

#include "scanner.h"

using BScanner::Symbologies;

int main(int, char**)
{
    const Symbologies::Symbology arr[] = {Symbologies::EAN13};
    std::vector<Symbologies::Symbology> symbs (arr, arr + sizeof(arr) / sizeof(arr[0]) );

    cv::Mat frame;
    BScanner::Scanner scanner(symbs);
    std::cout << "Opening camera..." << std::endl;
    cv::VideoCapture capture(0); // Test random numbers (e.g. 0-3) devices swap around whenever they feel like it

    // Set targeted FPS (image reads at FPS, but processing may take longer)
    int FPS = 30;
    capture.set(cv::CAP_PROP_FPS, (double)FPS); // May affect processing/decoding poorly due to forced timings

    if (!capture.isOpened())
    {
        std::cerr << "ERROR: Can't initialize camera capture" << std::endl;
        return 1;
    }

    // Create Windows
    cv::namedWindow("Frame", cv::WINDOW_NORMAL);

    for (;;)
    {
        capture >> frame; // read the next frame from camera
        if (frame.empty())
        {
            std::cerr << "ERROR: Can't grab camera frame." << std::endl;
            break;
        }
    
        BScanner::Result res = scanner.decodeRaw(frame);

        // Draw Rect on frame
        cv::Point2f vtx[4] = {res.coords[0], res.coords[1], res.coords[2], res.coords[3]};
        for (int i = 0; i < 4; i++)
        {
            line(frame, vtx[i], vtx[(i+1)%4], cv::Scalar(0,255,0), 2, cv::LINE_AA);
        }

        std::cout << res.value << std::endl;

        cv::putText( frame, res.value, res.coords[0], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0) );
        
        cv::imshow("Frame", frame);

        int key = cv::waitKey(1);
        if (key == 27/*ESC*/)
        {
            break;
        }
    }
    return 0;
}