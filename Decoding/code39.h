#pragma once

#include "decoder.h"

namespace BScanner
{
    class Code39 : public Decoder
    {
    private:
        std::string decodeValue(const cv::Mat row);
    public:
        Code39();
        ~Code39();
        BScanner::Result decode(const cv::Mat& img);
        std::vector<BScanner::Result> decodeMulti(const std::vector<cv::Mat> imgs);
        bool validate(const std::string value);
    };
}