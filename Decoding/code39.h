#include "decoder.h"

namespace Decoding
{
    class Code39 : public Decoder
    {
        public:
            Code39();
            void decode();
            void validate();
        private:
            std::vector<std::string> value; //decoded values
            std::vector<cv::Point> coords;  //barcode corner coordinates
    };
}