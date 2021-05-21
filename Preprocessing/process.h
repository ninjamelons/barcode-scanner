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
        void ThresholdPass(bool isNear, const cv::Mat& img,
            std::vector<std::tuple<cv::Mat, cv::RotatedRect>>& passes,
            Threshold thresh = Threshold());
        void GradientPass(const int angle);
    private:
        cv::RotatedRect FarThresholdPass(const cv::Mat& imgIn, Threshold thresh, int maxDim = 640);
        cv::RotatedRect NearThresholdPass(const cv::Mat& img);
    };
}