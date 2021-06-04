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
std::pair<std::string, std::pair<unsigned short, unsigned short>> Ean13::decodeValue(std::vector<int> row)
{
    std::string decoded = "";

    // Find barcode pixels (truncated from background)
    auto pair = getBarcodeLocation(row);
    auto firstBlackIndex = std::get<0>(pair);
    auto lastBlackIndex = std::get<1>(pair);

    int areaCount = 0;
    int areas[95] = {0};

    // Decode first 6 digits (45 areas per section (including start) + 5 middle areas)
    float pixelAreaHalf = (lastBlackIndex - firstBlackIndex) / 2;
    float pixelAreaHalfTwo = lastBlackIndex - pixelAreaHalf - firstBlackIndex;

    int pixelPerAreaHalf = (int) (pixelAreaHalf / 47.5);
    int pixelPerAreaHalfTwo = (int) (pixelAreaHalfTwo / 47.5);

    pixelAreaHalf = pixelAreaHalf - (2.5 * pixelPerAreaHalf);
    pixelAreaHalfTwo = pixelAreaHalfTwo + (2.5 * pixelPerAreaHalfTwo);

    int pixelPerAreaTotal = (int) ((lastBlackIndex - firstBlackIndex) / 95); // rounded down

    std::cout << std::to_string(firstBlackIndex) << "'" << std::to_string(lastBlackIndex) << std::endl;
    std::cout << std::to_string(pixelPerAreaHalf) <<';'<< std::to_string(pixelPerAreaHalfTwo) <<';'<< std::to_string(pixelPerAreaTotal) << std::endl;
    std::cout << std::to_string(pixelAreaHalf) <<';'<< std::to_string(pixelAreaHalfTwo) <<';'<< std::to_string(lastBlackIndex - firstBlackIndex) << std::endl;
    int currentArea = 1; // Black default
    for( int i = 1; i < 45; i++ )
    {

    }
    if(pixelAreaHalf > 30)
    {
        decoded = "Something";
    }

    //std::cout << std::endl;

    return std::make_pair(decoded, pair);
}

Symbologies::Symbology Ean13::getSymbology()
{
    return Symbologies::EAN13;
}