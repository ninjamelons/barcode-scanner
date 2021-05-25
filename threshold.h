#pragma once

#include <iostream>

namespace BScanner {
    class Threshold
    {
    public:
        Threshold(unsigned short blur = 3, unsigned short threshold = 115) : 
            blur_(blur), threshold_(threshold) {}
        ~Threshold(){}

        unsigned short GetBlur(){return blur_;}
        unsigned short GetThreshold(){return threshold_;}

        void SetBlur(unsigned short blur) {
            if( blur % 2 != 0 && blur != 0)
            {
                blur_ = blur;
            } else
            {
                blur_ = blur + 1;
            }
        }
        void SetThreshold(unsigned short threshold) {
            if( threshold <= 255 )
            {
                threshold_ = threshold;
            } else
            {
                threshold_ = 255;
            }
        }

        friend std::ostream& operator<<(std::ostream &out, Threshold& threshold);
    private:
        unsigned short blur_;
        unsigned short threshold_;
    };
}