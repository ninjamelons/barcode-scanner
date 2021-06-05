#pragma once

// C/C++ imports
#include <vector>

// OpenCV Imports
#include <opencv2/core/types.hpp>

#include "decoder.h"
#include "ean13.h"
#include "ean8.h"
#include "code39.h"
#include "code128.h"

using BScanner::Symbologies;

namespace BScanner
{
    class MultiDecoder {
        private:
            std::vector<std::unique_ptr<BScanner::Decoder>> decoders;
            void addSymbology(Symbologies::Symbology symbology);
        public:
            MultiDecoder(const std::vector<Symbologies::Symbology> symbologies);
            ~MultiDecoder();
            BScanner::Result decode(const cv::Mat& frame, std::vector<Symbologies::Symbology> symbologies);
            std::vector<Symbologies::Symbology> getSymbologies();
    };
}