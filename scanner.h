#pragma once

#include "multiDecoder.h"
#include "process.h"

using BScanner::Symbologies;

namespace BScanner
{
    class Scanner {
        private:
            BScanner::MultiDecoder decoder;
            BScanner::ImgProcess processor;
        public:
            Scanner(std::vector<Symbologies::Symbology> symbologies) :
                decoder{symbologies},
                processor{} {}
            ~Scanner() {}
            cv::Mat processRaw(const cv::Mat& frame, BScanner::Threshold thresh);
            BScanner::Result decodeRaw(const cv::Mat& frame);
            BScanner::Result decodeProcessed(const cv::Mat& processed);
    };
}