#pragma once

#include "decoder.h"

namespace BScanner
{
    class Code128 : public Decoder
    {
    private:
        std::pair<std::string, std::pair<unsigned short, unsigned short>> decodeValue(std::vector<int> row);
    public:
        Code128() {}
        ~Code128() {}
        bool validate(const std::string value);
        Symbologies::Symbology getSymbology();
    };
}