#pragma once

// C/C++ imports
#include <vector>

namespace BScanner
{
    class Decoder
    {   
    protected:
        Decoder() {} // protected constructor - abstract class
    private:
        virtual void decodeValues() = 0; // decodes specific row - symbology independant
    public:
        virtual void decode() = 0;
        virtual void validate() = 0;
        ~Decoder() {} // public destructor
    };
}