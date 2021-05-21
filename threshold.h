#pragma once

#include <iostream>

namespace BScanner {
    class Threshold
    {
    public:
        Threshold(unsigned short blur = 3, unsigned short threshold = 115);
        ~Threshold();

        unsigned short GetBlur();
        unsigned short GetThreshold();
        void SetBlur(unsigned short blur);
        void SetThreshold(unsigned short threshold);

        friend std::ostream& operator<<(std::ostream &out, Threshold& threshold);
    private:
        unsigned short blur_;
        unsigned short threshold_;
    };
}