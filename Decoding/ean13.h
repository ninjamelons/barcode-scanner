#pragma once

#include "decoder.h"

namespace BScanner
{
    class Ean13 : public Decoder
    {
    private:
        std::string decodeValue(const cv::Mat row);
    public:
        Ean13() {}
        ~Ean13() {}
        bool validate(const std::string value);
    };
}