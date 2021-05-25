#pragma once

#include "decoder.h"

namespace BScanner
{
    class Code39 : public Decoder
    {
    public:
        Code39();
        ~Code39();
        void decode();
        void validate();
    private:
        void decodeValues();
    };
}