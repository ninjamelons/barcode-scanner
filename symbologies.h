#pragma once

#include <iostream>

namespace BScanner {
    class Symbologies
    {
    public:
        enum Symbology {
            NONE = 0,
            EAN13,
            CODE39
        };

        friend std::ostream& operator<<(std::ostream & os, Symbology & sym)
        {
            switch(sym)
            {
                case 1:
                    return os << "EAN13";
                    break;
                case 2:
                    return os << "CODE39";
                    break;
                default:
                    return os << "None";
                    break;
            }
        }
    };
}