#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/videoio.hpp> // videocapture
#include <opencv2/highgui.hpp> // waitkey, namedwindow, createtrackbar, imshow

#include <iostream> // Print stuff

#include "threshold.h"

namespace BScanner {
    class ImgProcess
    {
    public:
        ImgProcess();
        void GradientPass(const int angle);
        cv::RotatedRect FarThresholdPass(const cv::Mat& imgIn, Threshold thresh = Threshold(), int maxDim = 720);
        cv::Mat NearThresholdPass(const cv::Mat& img, int outDims = 300);
    };
}