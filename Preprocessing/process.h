#pragma once

#include <opencv2/core.hpp>

namespace Preprocessing
{
    class ImgProcess
    {
    public:
        ImgProcess();
        void ThresholdPass(bool isNear, cv::Mat& img);
        void GradientPass(const int angle);
    private:
        void FarThresholdPass();
        void NearThresholdPass();
    };
}