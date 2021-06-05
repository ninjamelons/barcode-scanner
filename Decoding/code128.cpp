#include "code128.h"

using BScanner::Code128;

bool Code128::validate(const std::string value)
{
    bool isValid = true;
    if( value == "" )
    {
        isValid = false;
    } // Else if checksum does not match, set to false

    return isValid;
}

std::pair<std::string, std::pair<unsigned short, unsigned short>> Code128::decodeValue(std::vector<int> row)
{
    std::string decoded = "";

    // Find barcode pixels (truncated from background)
    auto pair = getBarcodeLocation(row);

    return std::make_pair(decoded, pair);
}

Symbologies::Symbology Code128::getSymbology()
{
    return Symbologies::CODE128;
}