#pragma once

#include <opencv2/core/types.hpp>

#include "symbologies.h"

using BScanner::Symbologies;

namespace BScanner {
    class Result
    {
    public:
        Result();
        ~Result();
        Symbologies::Symbology symbology;   //barcode symbology
        std::vector<cv::Point> coords;      //barcode corner coordinates
        std::string value;                  //barcode value
    };
}