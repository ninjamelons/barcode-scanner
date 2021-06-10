#include "ean13.h"

using BScanner::Ean13;

const int EDGE_GUARD[3] = { 1, 0, 1 };
const int CENTRE_GUARD[5] = { 0, 1, 0, 1, 0 };

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

const int PARITY[10][6] = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 1},
    {0, 0, 1, 1, 0, 1},
    {0, 0, 1, 1, 1, 0},
    {0, 1, 0, 0, 1, 1},
    {0, 1, 1, 0, 0, 1},
    {0, 1, 1, 1, 0, 0},
    {0, 1, 0, 1, 0, 1},
    {0, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 0},
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

    int pixelPerArea = (int) ((lastBlackIndex - firstBlackIndex) / 95); // rounded down
    pixelPerArea = pixelPerArea < 1 ? 1 : pixelPerArea; // minimum 1

    // Attempt Decode
    bool isUpsideDown = (row[firstBlackIndex] < 255);

    // If upside down, reverse vector
    if( isUpsideDown )
    {
        std::reverse(row.begin(), row.end());
    }

    //Get areas and lengths
    int lastPixel = row[firstBlackIndex];
    int currentAreaLength = 0;
    int areaCount = 1;
    /**
     * @brief
     * Start Guard: 3 sections
     * End Guard: 3 sections
     * Middle guard: 5 sections
     * Each digit: 4 sections
     * Total: 12 digits
     * (3*2) + (4*12) + 5
     */
    int sec[59] = {};

    for( int i = firstBlackIndex; i < lastBlackIndex + 1; i++ )
    {
        // Check if in next area
        int currentPixel = row[i];
        if( lastPixel != currentPixel )
        {
            int cArea = (int) ((currentAreaLength / pixelPerArea) + 0.5);
            if( areaCount - 1 < 59 )
            {
                sec[areaCount - 1] = cArea < 1 ? 1 : cArea;
            }

            areaCount++;
            currentAreaLength = 1;
        } else
        {
            currentAreaLength++;
        }

        lastPixel = currentPixel;
    }

    if( areaCount != 59 )
    {
        decoded = "";
    } else
    {
        // Get start and end guards
        int startPattern[3] = {sec[0], sec[1], sec[2]};
        int centrePattern[5] = {sec[27], sec[28], sec[29], sec[30], sec[31]};
        int endPattern[3] = {sec[56], sec[57], sec[58]};

        bool isStartGuard = matchPattern( startPattern, EDGE_GUARD, 3 );
        bool isCentreGuard = matchPattern( centrePattern,  CENTRE_GUARD, 5 );
        bool isEndGuard = matchPattern( endPattern, EDGE_GUARD, 3 );

        if( isStartGuard && isCentreGuard && isEndGuard)
        {
            std::string startSix = "";
            std::string endSix = "";
            int encodings[6] = {};

            // Loop through encodings
            for( int i = 0; i < 10; i++ )
            {
                // Decode first 6 digits (Excluding start guard, centre, end, end guard)
                for( int j = 0; j < 6; j++ )
                {
                    // Each digit is 4 sections, +2 indexes for start guard
                    int areaStart = (j * 4) + 2;
                    int pattern[4] = {sec[areaStart], sec[areaStart+1], sec[areaStart+2], sec[areaStart+3]};
                    if( matchPattern( pattern, LR_CODE[i], 4 ) )
                    {
                        startSix + std::to_string(i);
                        encodings[j] = 0;
                    }
                    else if ( matchPattern( pattern, G_CODE[i], 4 ))
                    {
                        startSix + std::to_string(i);
                        encodings[j] = 1;
                    }
                }

                // Decode last 6 digits (Excluding start guard, start, centre, end guard)
                for( int j = 0; j < 6; j++ )
                {
                    // Each digit is 4 sections, +2 indexes for start guard, +24 start digits, +5 centre digits
                    int areaStart = (j * 4) + 31;
                    int pattern[4] = {sec[areaStart], sec[areaStart+1], sec[areaStart+2], sec[areaStart+3]};
                    if( matchPattern( pattern, LR_CODE[i], 4 ) )
                    {
                        startSix + std::to_string(i);
                    }      
                }
            }

            // Get first digit
            for( int i = 0; i < 10; i++ )
            {
                if( matchPattern( encodings, PARITY[i], 6) )
                {
                    decoded = std::to_string(i);
                    break;
                }
            }

            decoded += startSix + endSix;
        } else
        {
            decoded = "";
        }
    }

    return std::make_pair(decoded, pair);
}

Symbologies::Symbology Ean13::getSymbology()
{
    return Symbologies::EAN13;
}

bool Ean13::matchPattern(const int character[], const int pattern[], int size)
{
    for( int i = 0; i < size; i++ )
    {
        if( character[i] != pattern[i] )
        {
            return false;
        }
    }
    return true;
}