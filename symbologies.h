#pragma once

#include <iostream>

namespace BScanner {
    class Symbologies
    {
    public:
        enum Symbology {
            NONE = 0,
            EAN13,
            EAN8,
            CODE39,
            CODE128
        };

        friend std::ostream& operator<<(std::ostream & os, Symbology & sym)
        {
            switch(sym)
            {
                case 1:
                    return os << "EAN13";
                    break;
                case 2:
                    return os << "EAN8";
                    break;
                case 3:
                    return os << "CODE39";
                    break;
                case 4:
                    return os << "CODE128";
                    break;
                default:
                    return os << "None";
                    break;
            }
        }
    };
}