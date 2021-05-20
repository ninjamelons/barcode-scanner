#include "process.h"

#include <iostream>

namespace Preprocessing
{
    ImgProcess::ImgProcess() {

    }

    void ImgProcess::ThresholdPass(bool isNear, cv::Mat& img)
    {
        if( isNear )
        {
            NearThresholdPass(img);
        } else
        {
            FarThresholdPass(img);
            NearThresholdPass(img);
        }
    }

    void ImgProcess::GradientPass(const int angle)
    {
        std::cout << "Gradient Pass" << std::endl;
    }

    void ImgProcess::FarThresholdPass(cv::Mat& img)
    {
        std::cout << "Far Pass" << std::endl;
    }

    void ImgProcess::NearThresholdPass(cv::Mat& img)
    {
        std::cout << "Near Pass" << std::endl;
    }
}