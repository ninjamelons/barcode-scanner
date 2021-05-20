#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()

#include <iostream>
#include <vector>

using namespace cv;
using std::cout; using std::cerr; using std::endl; using std::vector;

Mat ProcessMatImage(cv::Mat &img, int threshold, int blur, int angle);

int main(int, char**)
{
    Mat frame;
    cout << "Opening camera..." << endl;
    VideoCapture capture(0); // open the first camera

    // Set initial FPS
    int FPS = 30;
    capture.set(CAP_PROP_FPS, (double)FPS);

    if (!capture.isOpened())
    {
        cerr << "ERROR: Can't initialize camera capture" << endl;
        return 1;
    }

    cout << "Frame width: " << capture.get(CAP_PROP_FRAME_WIDTH) << endl;
    cout << "     height: " << capture.get(CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "Capturing FPS: " << capture.get(CAP_PROP_FPS) << endl;

    cout << endl << "Press 'ESC' to quit, 'space' to toggle frame processing" << endl;
    cout << endl << "Start grabbing..." << endl;

    // Create Windows
    namedWindow("Frame", WINDOW_NORMAL);
    namedWindow("Processed", WINDOW_NORMAL);

    // Setup trackbar vars
    const int threshold_max = 255;
    const int blur_max = 31;
    const int angle_max = 360;

    int threshold_slider = 115;
    int blur_slider = 15;
    int angle_slider = 15;

    int threshold;
    int blur;
    int angle;

    size_t nFrames = 0;
    bool enableProcessing = false;
    int64 t0 = cv::getTickCount();
    int64 processingTime = 0;
    for (;;)
    {
        capture >> frame; // read the next frame from camera
        if (frame.empty())
        {
            cerr << "ERROR: Can't grab camera frame." << endl;
            break;
        }
        nFrames++;
        int coutps = 5;
        if (nFrames % (int)(FPS * coutps + 0.5) == 0)
        {
            int64 t1 = cv::getTickCount();
            cout << "Frames captured: " << cv::format("%5lld", (long long int)nFrames)
                 << "    Average FPS: " << cv::format("%9.1f", (double)getTickFrequency() * FPS * coutps / (t1 - t0))
                 << "    Average time per frame: " << cv::format("%9.2f ms", (double)(t1 - t0) * 1000.0f / (FPS * coutps * getTickFrequency()))
                 << "    Average processing time: " << cv::format("%9.2f ms", (double)(processingTime) * 1000.0f / (FPS * coutps * getTickFrequency()))
                 << std::endl;
            t0 = t1;
            processingTime = 0;
        }
        if (!enableProcessing)
        {
            imshow("Frame", frame);
        }
        else
        {
            char ThresholdTrackbar[50];
            sprintf( ThresholdTrackbar, "Threshold x %d", threshold_max );
            createTrackbar( ThresholdTrackbar, "Frame", &threshold_slider, threshold_max );

            char BlurTrackbar[50];
            sprintf( BlurTrackbar, "Blur x %d", blur_max );
            createTrackbar( BlurTrackbar, "Frame", &blur_slider, blur_max );

            char AngleTrackbar[50];
            sprintf( AngleTrackbar, "Blur x %d", angle_max );
            createTrackbar( AngleTrackbar, "Frame", &angle_slider, angle_max );

            // Start processing tick count
            int64 tp0 = cv::getTickCount();

            if(blur_slider % 2 == 0) {
                blur = blur_slider + 1;
            } else {
                blur = (int) blur_slider;
            }

            threshold = (int) threshold_slider;
            angle = (int) angle_slider;

            // Process Frame
            Mat processed = ProcessMatImage(frame, threshold, blur, angle);


            //cv::Canny(frame, processed, 400, 1000, 5);
            
            processingTime += cv::getTickCount() - tp0;

            imshow("Frame", processed);
            imshow("Processed", frame);
        }

        int key = waitKey(1);
        if (key == 27/*ESC*/)
        {
            break;
        } else if (key == 32/*SPACE*/)
        {
            enableProcessing = !enableProcessing;
            cout << "Enable frame processing ('space' key): " << enableProcessing << endl;
        }
    }
    std::cout << "Number of captured frames: " << nFrames << endl;
    return nFrames > 0 ? 0 : 1;
}

bool sortContourArea (vector<Point> i, vector<Point> j) { return (contourArea(i) > contourArea(j)); }

Mat ProcessMatImage(cv::Mat &img, int threshold, int blur, int angle) {
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
    cv::Sobel(img, grad_x, ddepth, 1, 0, -1);
    cv::Sobel(img, grad_y, ddepth, 0, 1, -1);

    // 1st image derivative w/ Scharr op
    //cv::Scharr(img, grad_x, ddepth, 1, 0);
    //cv::Scharr(img, grad_y, ddepth, 0, 1);

    // converting back to CV_8U
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    // Display gradiented img
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, img);

    // Blur to prepare for threshold
    cv::blur(img, img, Size(blur, blur));

    // Threshold gradient image
    cv::threshold(img, img, threshold, 255, THRESH_BINARY);

    // Fill larger gaps between lines
    Mat kernel;
    kernel = cv::getStructuringElement(MORPH_RECT, Size(21, 7));

    cv::morphologyEx(img, img, MORPH_CLOSE, kernel);

    // Remove noisy artifacting
    cv::erode(img, img, kernel, Point(-1,-1), 2, BORDER_CONSTANT, morphologyDefaultBorderValue());

    // Enlarge main focus
    cv::dilate(img, img, kernel, Point(-1,-1), 8, BORDER_CONSTANT, morphologyDefaultBorderValue());

    // Find and Create largest bounding box (contours)
    cv::Mat ogScale;
    cv::resize(img, ogScale, raw.size(), 0, 0);

    vector<vector<Point>> imgContours;
    vector<Vec4i> hierarchy;
    cv::findContours( ogScale, imgContours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );

    std::sort(imgContours.begin(), imgContours.end(), sortContourArea);

    Point2f vtx[4];
    if(!imgContours.empty()) {
        RotatedRect rect = cv::minAreaRect(imgContours[0]);
        rect.points(vtx);

        for (int i = 0; i < 4; i++)
        {
            line(raw, vtx[i], vtx[(i+1)%4], Scalar(0,255,0), 5, LINE_AA);
        }
    }

    return raw;
}