#pragma once

// C/C++ imports
#include <vector>

// OpenCV Imports
#include <opencv2/core/types.hpp>

#include "symbologies.h"
#include "decoder.h"

namespace BScanner
{
    class MultiDecoder {
        private:
            std::vector<std::unique_ptr<BScanner::Decoder>> decoders; // Single point of access
        public:
            void decode();
            MultiDecoder(const std::vector<BScanner::Symbologies> symbologies);
            ~MultiDecoder();
    };
}