#pragma once

#include "decoder.h"

namespace BScanner
{
    class Ean8 : public Decoder
    {
    private:
        std::pair<std::string, std::pair<unsigned short, unsigned short>> decodeValue(std::vector<int> row);
        bool validate(const std::string value);
    public:
        Ean8() {}
        ~Ean8() {}
        Symbologies::Symbology getSymbology();
    };
}