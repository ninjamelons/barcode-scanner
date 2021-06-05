#include "ean8.h"

using BScanner::Ean8;

bool Ean8::validate(const std::string value)
{
    bool isValid = true;
    if( value == "" )
    {
        isValid = false;
    } // Else if checksum does not match, set to false

    return isValid;
}

std::pair<std::string, std::pair<unsigned short, unsigned short>> Ean8::decodeValue(std::vector<int> row)
{
    std::string decoded = "";

    // Find barcode pixels (truncated from background)
    auto pair = getBarcodeLocation(row);

    return std::make_pair(decoded, pair);
}

Symbologies::Symbology Ean8::getSymbology()
{
    return Symbologies::EAN8;
}