#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace BScanner {
    class ImgProcess
    {
    public:
        ImgProcess();
        void ThresholdPass(bool isNear, const cv::Mat& img, std::vector<cv::Mat> passes);
        void GradientPass(const int angle);
    private:
        cv::Mat FarThresholdPass(cv::Mat& img);
        cv::Mat NearThresholdPass(cv::Mat& img);
    };
}