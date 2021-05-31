#pragma once

#include <iostream>

namespace BScanner {
    class Threshold
    {
    public:
        Threshold(unsigned short blur = 3, unsigned short threshold = 115
                , unsigned short kernelx = 15, unsigned short kernely = 7
                , float erode = 8.0f, float dilate = 8.0f) : 
            blur_(blur), threshold_(threshold), kernelx_(kernelx), kernely_(kernely), erode_(erode), dilate_(dilate) {}
        ~Threshold(){}

        unsigned short GetBlur(){return blur_;}
        unsigned short GetThreshold(){return threshold_;}
        unsigned short GetKernelX(){return kernelx_;}
        unsigned short GetKernelY(){return kernely_;}
        float GetErode(){return erode_;}
        float GetDilate(){return dilate_;}

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

        void SetKernelXY(unsigned short kernelx, unsigned short kernely) {
            SetKernelX(kernelx);
            SetKernelY(kernely);
        }

        void SetKernelX(unsigned short kernelx) {
            if( kernelx % 2 != 0 && kernelx != 0)
            {
                kernelx_ = kernelx;
            } else
            {
                kernelx_ = kernelx + 1;
            }
        }

        void SetKernelY(unsigned short kernely) {
            if( kernely % 2 != 0 && kernely != 0)
            {
                kernely_ = kernely;
            } else
            {
                kernely_ = kernely + 1;
            }
        }

        // Rounds to nearest half
        void SetErode(float erode) {
            erode_ = (floor((erode*2)+0.5)/2);
        }

        void SetDilate(float dilate) {
            dilate_ = (floor((dilate*2)+0.5)/2);
        }

        friend std::ostream& operator<<(std::ostream &os, Threshold& th)
        {
            return os << std::to_string(th.GetThreshold());
        }
    private:
        unsigned short blur_;
        unsigned short threshold_;
        unsigned short kernelx_;
        unsigned short kernely_;
        float erode_;
        float dilate_;
    };
}