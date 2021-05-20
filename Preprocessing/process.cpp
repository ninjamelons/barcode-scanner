#include "process.h"

#include <iostream>

using BScanner::ImgProcess;

ImgProcess::ImgProcess() {

}

void ImgProcess::ThresholdPass(bool isNear, const cv::Mat& imgIn, std::vector<cv::Mat> passes)
{
    cv::Mat input = imgIn;
    if( isNear )
    {
        passes.push_back(NearThresholdPass(input));
    } else
    {
        passes.push_back(FarThresholdPass(input));
        passes.push_back(NearThresholdPass(input));
    }
}

void ImgProcess::GradientPass(const int angle)
{
    std::cout << "Gradient Pass" << std::endl;
}

cv::Mat ImgProcess::FarThresholdPass(cv::Mat& img)
{
    cv::Mat 
    const int maxLgDim = 640;
    // Get largest dimension
    if( img.size().aspectRatio() >= 1 ) //aspect = (width/height)
    {
        // Width largest
        if( !(img.size().width <= maxLgDim) )
        {
            cv::resize(img, img, cv::Size(maxLgDim, (int)maxLgDim/img.size().aspectRatio()));
        }
    }
    else
    {
        // Height largest
        if( !(img.size().height <= maxLgDim) )
        {
            cv::resize(img, img, cv::Size((int)maxLgDim/img.size().aspectRatio(), maxLgDim));
        }
    }

    std::cout << "Far Pass" << std::endl;
    return img;
}

cv::Mat ImgProcess::NearThresholdPass(cv::Mat& img)
{
    std::cout << "Near Pass" << std::endl;
    return img;
}