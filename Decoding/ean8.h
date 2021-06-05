#pragma once

#include "decoder.h"

namespace BScanner
{
    class Ean8 : public Decoder
    {
    private:
        std::pair<std::string, std::pair<unsigned short, unsigned short>> decodeValue(std::vector<int> row);
    public:
        Ean8() {}
        ~Ean8() {}
        bool validate(const std::string value);
        Symbologies::Symbology getSymbology();
    };
}