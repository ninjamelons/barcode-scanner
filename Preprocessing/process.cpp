#include "process.h"

#include <iostream>

namespace Preprocessing
{
    ImgProcess::ImgProcess() {

    }

    void ImgProcess::ThresholdPass(bool isNear, cv::Mat& img)
    {
        std::cout << "Threshold Pass" << std::endl;
    }

    void ImgProcess::GradientPass(const int angle)
    {
        std::cout << "Gradient Pass" << std::endl;
    }

    void ImgProcess::FarThresholdPass()
    {
        std::cout << "Far Pass" << std::endl;
    }

    void ImgProcess::NearThresholdPass()
    {
        std::cout << "Near Pass" << std::endl;
    }
}