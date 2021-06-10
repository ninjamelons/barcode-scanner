#pragma once

#include "decoder.h"

namespace BScanner
{
    class Ean13 : public Decoder
    {
    private:
        std::pair<std::string, std::pair<unsigned short, unsigned short>>
            decodeValue(std::vector<int> row);
        bool validate(const std::string value);
        bool matchPattern(const int character[], const int pattern[], int size);
    public:
        Ean13() {}
        ~Ean13() {}
        Symbologies::Symbology getSymbology();
    };
}