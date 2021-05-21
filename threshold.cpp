#include "threshold.h"

using BScanner::Threshold;

Threshold::Threshold(unsigned short blur, unsigned short threshold) : 
    blur_(blur), threshold_(threshold) {}

Threshold::~Threshold() {}

unsigned short Threshold::GetBlur()
{
    return blur_;
}

unsigned short Threshold::GetThreshold()
{
    return threshold_;
}

void Threshold::SetBlur(unsigned short blur)
{
    if( blur % 2 == 0 && blur != 0)
    {
        blur_ = blur;
    } else
    {
        blur_ = blur + 1;
    }
}

void Threshold::SetThreshold(unsigned short threshold)
{
    if( threshold <= 255 )
    {
        threshold_ = threshold;
    } else
    {
        threshold_ = 255;
    }
}