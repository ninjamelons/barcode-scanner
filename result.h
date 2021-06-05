#pragma once

#include <opencv2/core/types.hpp>

#include "symbologies.h"

using BScanner::Symbologies;

namespace BScanner {
    class Result
    {
    public:
        Result() {}
        Result(Symbologies::Symbology sym, std::array<cv::Point, 4> cds, std::string val)
            : symbology(sym), coords{cds}, value(val) {}
        ~Result() {}
        Symbologies::Symbology symbology;   //barcode symbology
        std::array<cv::Point, 4> coords;    //barcode corner coordinates
        std::string value;                  //barcode value
    };
}