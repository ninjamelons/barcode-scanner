#include "multiDecoder.h"

using BScanner::MultiDecoder;

MultiDecoder::MultiDecoder(const std::vector<Symbologies::Symbology> symbologies)
{
    for( int i = 0; i < symbologies.size(); i++ )
    {
        addSymbology(symbologies[i]);
    }
}
MultiDecoder::~MultiDecoder() {}

/**
 * @brief Returns first decoded and validated value
 * 
 * @param frame 
 * @param symbologies 
 * @return BScanner::Result 
 */
BScanner::Result MultiDecoder::decode(const cv::Mat& frame)
{
    BScanner::Result res;

    for( auto it = decoders.begin(); it != decoders.end(); it++ )
    {
        BScanner::Result tempRes = (*it)->decode(frame);
        if( tempRes.value != "" )
        {
            res = tempRes;
            break;
        }
    }

    return res;
}

std::vector<Symbologies::Symbology> MultiDecoder::getSymbologies()
{
    std::vector<Symbologies::Symbology> symbologies;

    for( int i = 0; i < decoders.size(); i++ )
    {
        symbologies.push_back(decoders[i]->getSymbology());
    }

    return symbologies;
}

void MultiDecoder::addSymbology(Symbologies::Symbology symbology)
{
    switch( symbology )
    {
        case Symbologies::EAN13:
            decoders.push_back(std::make_unique<BScanner::Ean13>());
            break;
        case Symbologies::EAN8:
            decoders.push_back(std::make_unique<BScanner::Ean8>());
            break;
        case Symbologies::CODE39:
            decoders.push_back(std::make_unique<BScanner::Code39>());
            break;
        case Symbologies::CODE128:
            decoders.push_back(std::make_unique<BScanner::Code128>());
            break;
    }
}