#include "ean13.h"

using BScanner::Ean13;

// R_CODE is the L_CODE but inverted (black is white)
// L_CODE starts with white
const int LR_CODE[10][4] = {
    {3, 2, 1, 1},
    {2, 2, 2, 1},
    {2, 1, 2, 2},
    {1, 4, 1, 1},
    {1, 1, 3, 2},
    {1, 2, 3, 1},
    {1, 1, 1, 4},
    {1, 3, 1, 2},
    {1, 2, 1, 3},
    {3, 1, 1, 2} 
};

const int G_CODE[10][4] = {
    {1, 1, 2, 3},
    {1, 2, 2, 2},
    {2, 2, 1, 2},
    {1, 1, 4, 1},
    {2, 3, 1, 1},
    {1, 3, 2, 1},
    {4, 1, 1, 1},
    {2, 1, 3, 1},
    {3, 1, 2, 1},
    {2, 1, 1, 3} 
};

bool Ean13::validate(const std::string value)
{
    bool isValid = true;
    if( value == "" )
    {
        isValid = false;
    } // Else if checksum does not match, set to false

    return isValid;
}

/**
 * @brief Finds start and end position of barcode
 * 
 * @param row 
 * @return std::string 
 */
std::string Ean13::decodeValue(const cv::Mat row)
{
    std::string decoded = "";

    // Find barcode pixels (truncated from background)
    int firstBlackIndex = -1;
    int lastBlackIndex = -1;
    int whiteCount = 0;
    int whiteMax = 15;
    int areaCount = 0;
    bool barcodeFound = false;
    int areas[95] = {0};

    for( int i = 0; i < row.cols; i++ )
    {
        int pixel = row.at<uchar>(i,0);

        if( pixel == 255 )                  // BLACK PIXEL
        {
            if( !barcodeFound )
            {
                if( whiteCount >= whiteMax )
                {
                    firstBlackIndex = i;
                    whiteCount = 0;
                    barcodeFound = true;
                }
            } else
            {
                lastBlackIndex = i;
                whiteCount = 0;
            }
        } else                              // WHITE PIXEL
        {
            whiteCount++;
            if( barcodeFound )
            {
                if( whiteCount >= whiteMax )
                {
                    break; // No need to continue, barcode is fully encapsulated
                }
            }
        }
    }

    // Decode first 6 digits (45 areas per section (including start) + 5 middle areas)
    float pixelAreaHalf = (lastBlackIndex - firstBlackIndex) / 2;
    float pixelAreaHalfTwo = lastBlackIndex - pixelAreaHalf - firstBlackIndex;

    int pixelPerAreaHalf = (int) (pixelAreaHalf / 47.5);
    int pixelPerAreaHalfTwo = (int) (pixelAreaHalfTwo / 47.5);

    pixelAreaHalf = pixelAreaHalf - (2.5 * pixelPerAreaHalf);
    pixelAreaHalfTwo = pixelAreaHalfTwo + (2.5 * pixelPerAreaHalfTwo);

    int pixelPerAreaTotal = (int) ((lastBlackIndex - firstBlackIndex) / 95); // rounded down

    std::cout << std::to_string(pixelPerAreaHalf) <<';'<< std::to_string(pixelPerAreaHalfTwo) <<';'<< std::to_string(pixelPerAreaTotal) << std::endl;
    std::cout << std::to_string(pixelAreaHalf) <<';'<< std::to_string(pixelAreaHalfTwo) <<';'<< std::to_string(lastBlackIndex - firstBlackIndex) << std::endl;
    int currentArea = 1; // Black default
    for( int i = 1; i < 45; i++ )
    {
    }

    //std::cout << std::endl;

    return decoded;
}