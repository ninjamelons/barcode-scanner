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
        cv::RotatedRect GradientPass(int angleSpacing, int maxDim = 720);
        cv::RotatedRect FarThresholdPass(const cv::Mat& imgIn,
            Threshold thresh = Threshold(), int maxDim = 720);
        cv::Mat TransformRotated(const cv::Mat& img, cv::RotatedRect rect);
        cv::Mat PrepareRegion(const cv::Mat& img, int outDims = 300);
    };
}

