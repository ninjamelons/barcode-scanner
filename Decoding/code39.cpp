#include "code39.h"

using BScanner::Code39;

Code39::Code39(){}

Code39::~Code39(){}

/**
 * @brief Decode single barcode from image
 * 
 * @param img 
 * @return BScanner::Result 
 */
BScanner::Result Code39::decode(const cv::Mat& img)
{
    // Decode row per row
    cv::Mat row;                                        // Row to decode
    std::vector<std::string> modes;                     // Unique elements in values
    std::vector<int> modes_count;                       // Unique element counts in values
    std::vector<std::tuple<std::string, int>> values;   // Decoded values (Many will be wrong)
    std::string value = "";                             // Decoded value (Correct or empty)
    std::array<cv::Point, 4> coords;                    // Corners of barcode
    
    for( int i = 0; i < img.rows; i++ )
    {
        row = img.row(i);
        auto tmpDecoded = decodeValue(row);
        bool isValid = validate(tmpDecoded);
        if( isValid )
        {
            // Determine if value in modes/modes_count
            auto it = std::find(modes.begin(), modes.end(), tmpDecoded);
            if( it != modes.end() ) // If iterator not at end, then element is found
            {
                int index = it - modes.begin();
                modes_count[index]++;
            } else
            {
                modes.push_back(tmpDecoded);
                modes_count.push_back(0);
            }

            values.push_back(std::make_tuple(tmpDecoded, i));
        }
    }

    if( modes_count.size() > 0 )
    {
        auto itMax = std::max_element(modes_count.begin(), modes_count.end());
        int index = itMax - modes_count.begin();
        value = modes[index];
    }

    return BScanner::Result(Symbologies::CODE39, coords, value);
}

/**
 * @brief Decode multiple barcode images
 * 
 * @param imgs Close up vector of individual barcodes
 * @return std::vector<BScanner::Result> 
 */
std::vector<BScanner::Result> Code39::decodeMulti(const std::vector<cv::Mat> imgs)
{
    std::vector<BScanner::Result> results;

    for( cv::Mat img : imgs )
    {
        results.push_back(decode(img));
    }

    return results;
}

bool Code39::validate(const std::string value)
{
    bool isValid = true;
    if( value == "" )
    {
        isValid = false;
    } // Else if checksum does not match, set to false

    return isValid;
}

std::string Code39::decodeValue(const cv::Mat row)
{
    return "";
}