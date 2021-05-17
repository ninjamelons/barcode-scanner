#pragma once

// C/C++ imports
#include <vector>

// OpenCV Imports
#include <opencv2/core/types.hpp>

namespace Decoding
{
    class Decoder {
        protected:
            Decoder() {} // protected constructor - abstract class
        private:
            std::vector<Decoder> decoders;
            void decodeValues(); // decodes specific row - symbology independant
        public:
            virtual void decode() = 0;
            virtual void validate() = 0;
            ~Decoder(); // public destructor
    };
}