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
std::string Code39::decodeValue(const cv::Mat row)
{
    std::string decoded = "";

    int firstBlack = -1;
    int whiteCount = 0;
    bool barcodeFound = false;

    for( int i = 0; i < row.cols; i++ )
    {
        //std::cout << std::to_string(row.at<uchar>(i,0));
    }
    //std::cout << std::endl;

    return decoded;
}