#pragma once

#include "decoder.h"

namespace BScanner
{
    class Code39 : public Decoder
    {
    private:
        std::string decodeValue(const cv::Mat row);
    public:
        Code39() {}
        ~Code39() {}
        bool validate(const std::string value);
    };
}