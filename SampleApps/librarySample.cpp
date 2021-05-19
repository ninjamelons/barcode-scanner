#include <iostream>
#include <vector>

#include "process.h"
#include "decoder.h"

int main(int, char**)
{
    Mat frame;
    cout << "Opening camera..." << endl;
    VideoCapture capture(2); // open the first camera

    // Set initial FPS
    int FPS = 30;
    capture.set(CAP_PROP_FPS, (double)FPS);

    if (!capture.isOpened())
    {
        cerr << "ERROR: Can't initialize camera capture" << endl;
        return 1;
    }

    // Create Windows
    namedWindow("Frame", WINDOW_NORMAL);
    namedWindow("One", WINDOW_NORMAL);
    namedWindow("Two", WINDOW_NORMAL);

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
            cerr << "ERROR: Can't grab camera frame." << endl;
            break;
        }

        char OneThresholdTrackbar[50];
        sprintf( OneThresholdTrackbar, "Threshold x %d", one_threshold_max );
        createTrackbar( OneThresholdTrackbar, "One", &one_threshold_slider, one_threshold_max );
        char OneBlurTrackbar[50];
        sprintf( OneBlurTrackbar, "Blur x %d", one_blur_max );
        createTrackbar( OneBlurTrackbar, "One", &one_blur_slider, one_blur_max );

        char TwoThresholdTrackbar[50];
        sprintf( TwoThresholdTrackbar, "Threshold x %d", two_threshold_max );
        createTrackbar( TwoThresholdTrackbar, "Two", &two_threshold_slider, two_threshold_max );
        char TwoBlurTrackbar[50];
        sprintf( TwoBlurTrackbar, "Blur x %d", two_blur_max );
        createTrackbar( TwoBlurTrackbar, "Two", &two_blur_slider, two_blur_max );

        if(one_blur_slider % 2 == 0) {
            one_blur = one_blur_slider + 1;
        } else {
            one_blur = (int) one_blur_slider;
        }
        one_threshold = (int) one_threshold_slider;
        one_angle = (int) one_angle_slider;

        if(two_blur_slider % 2 == 0) {
            two_blur = two_blur_slider + 1;
        } else {
            two_blur = (int) two_blur_slider;
        }
        two_threshold = (int) two_threshold_slider;
        two_angle = (int) two_angle_slider;

        
        // Process Frame
        Mat passOne = Preprocessing::PassOne(frame, threshold, blur);
        Mat passTwo = Preprocessing::PassTwo(frame, threshold, blur);
        Mat processed = ProcessMatImage(frame, threshold, blur, angle);
        
        imshow("Frame", processed);
        imshow("One", frame);
        imshow("Two", frame);

        int key = waitKey(1);
        if (key == 27/*ESC*/)
        {
            break;
        }
    }
    return 0;
}