#pragma once

#include <opencv2/core.hpp>

// C/C++ imports
#include <vector>

#include "result.h"
#include "symbologies.h"

namespace BScanner
{
    class Decoder
    {   
    protected:
        Decoder() {} // protected constructor - abstract class
        std::pair<unsigned short, unsigned short> getBarcodeLocation(std::vector<int> row);
    private:
        virtual std::pair<std::string, std::pair<unsigned short, unsigned short>> decodeValue(std::vector<int> row) = 0; // decodes specific row
    public:
        BScanner::Result decode(const cv::Mat& img);
        std::vector<BScanner::Result> decodeMulti(const std::vector<cv::Mat> imgs);
        virtual bool validate(const std::string value) = 0;
        virtual Symbologies::Symbology getSymbology() = 0;
        ~Decoder() {} // public destructor
    };
}