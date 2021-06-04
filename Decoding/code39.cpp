#include "code39.h"

using BScanner::Code39;

bool Code39::validate(const std::string value)
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
std::pair<std::string, std::pair<unsigned short, unsigned short>> Code39::decodeValue(std::vector<int> row)
{
    std::string decoded = "";

    return std::make_pair(decoded, std::make_pair(0,0));
}