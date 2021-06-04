#pragma once

#include "decoder.h"

namespace BScanner
{
    class Code39 : public Decoder
    {
    private:
        std::pair<std::string, std::pair<unsigned short, unsigned short>> decodeValue(std::vector<int> row);
    public:
        Code39() {}
        ~Code39() {}
        bool validate(const std::string value);
    };
}