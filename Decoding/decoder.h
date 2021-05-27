#pragma once

#include <opencv2/core.hpp>

// C/C++ imports
#include <vector>

#include "result.h"

namespace BScanner
{
    class Decoder
    {   
    protected:
        Decoder() {} // protected constructor - abstract class
    private:
        virtual std::string decodeValue(const cv::Mat row) = 0; // decodes specific row
    public:
        BScanner::Result decode(const cv::Mat& img);
        std::vector<BScanner::Result> decodeMulti(const std::vector<cv::Mat> imgs);
        virtual bool validate(const std::string value) = 0;
        ~Decoder() {} // public destructor
    };
}