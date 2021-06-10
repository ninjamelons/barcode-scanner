#include "decoder.h"

using BScanner::Decoder;

Decoder::~Decoder() {}

/**
 * @brief 
 * 
 * @param img 
 * @return BScanner::Result 
 */
BScanner::Result Decoder::decode(const cv::Mat& img)
{
    // Decode row per row
    std::vector<int> row;                               // Row to decode
    std::vector<std::string> modes;                     // Unique elements in values
    std::vector<int> modes_count;                       // Unique element counts in values
    std::string value = "";                             // Decoded value (Correct or empty)
    std::array<cv::Point, 4> coords = {};               // Corners of barcode
    bool isFirstValidRow = false;

    for( int i = 0; i < img.rows; i++ )
    {
        row.clear();
        for( int j = 0; j < img.cols; j++ )
        {
            row.push_back(img.at<uchar>(i, j));
        }

        auto tmpDecoded = decodeValue(row);
        auto stringDecoded = std::get<0>(tmpDecoded);
        bool isValid = validate(stringDecoded);
        if( isValid )
        {
            // Get corners
            auto tmpPoint = std::get<1>(tmpDecoded);
            if( !isFirstValidRow )
            {
                coords[1] = cv::Point(std::get<0>(tmpPoint), i);
                coords[2] = cv::Point(std::get<1>(tmpPoint), i);
                isFirstValidRow = true;
            } else {
                coords[0] = cv::Point(std::get<0>(tmpPoint), i);
                coords[3] = cv::Point(std::get<1>(tmpPoint), i);
            }

            // Determine if value in modes/modes_count
            auto it = std::find(modes.begin(), modes.end(), stringDecoded);
            if( it != modes.end() ) // If iterator not at end, then element is found
            {
                int index = it - modes.begin();
                modes_count[index]++;
            } else
            {
                modes.push_back(stringDecoded);
                modes_count.push_back(0);
            }
        }
    }

    if( modes_count.size() > 0 )
    {
        auto itMax = std::max_element(modes_count.begin(), modes_count.end());
        int index = itMax - modes_count.begin();
        value = modes[index];
    }

    return BScanner::Result(getSymbology(), coords, value);
}

/**
 * @brief Decode multiple barcode images
 * 
 * @param imgs Close up vector of individual barcodes
 * @return std::vector<BScanner::Result> 
 */
std::vector<BScanner::Result> Decoder::decodeMulti(const std::vector<cv::Mat> imgs)
{
    std::vector<BScanner::Result> results;

    for( cv::Mat img : imgs )
    {
        results.push_back(decode(img));
    }

    return results;
}

std::pair<unsigned short, unsigned short> Decoder::getBarcodeLocation(std::vector<int> row)
{
    unsigned short firstBlackIndex = 0;
    unsigned short lastBlackIndex = 0;
    int areaCount = 0;
    int sameCount = 0;
    int sameMax = 20;
    int maxColour = 0;
    int lastColour = 0;
    bool barcodeFound = false;

    for( int i = 0; i < row.size(); i++ )
    {
        int pixel = row[i];

        if( lastColour == pixel )
        {
            sameCount++;
            if( !barcodeFound && sameCount >= sameMax ) // Same as last pixel or lots of same and barcode not found
            {
                maxColour = pixel;
                continue;
            }
            if( barcodeFound && sameCount >= sameMax && maxColour == pixel) {
                lastBlackIndex = i - sameCount;
                break;
            }
        } else 
        {
            //Larger than possible barcode
            if( areaCount > 59 )
            {
                barcodeFound = false;
                firstBlackIndex = 0;
                lastBlackIndex = 0;
            }

            if( sameCount >= sameMax ) {
                firstBlackIndex = i;
                barcodeFound = true;
            }
            if( barcodeFound )
            {
                areaCount++;
                if( sameCount < sameMax )
                {
                    lastBlackIndex = i - 1; // -1 due to going over barcode by 1
                }
            }

            sameCount = 1;
        }
        lastColour = pixel;
    }
    
    return std::make_pair(firstBlackIndex, lastBlackIndex);
}