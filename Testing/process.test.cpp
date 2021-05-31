#include <catch2/catch.hpp> // test case

#include <opencv2/core.hpp> // core / cv scope
#include <opencv2/imgcodecs.hpp> // imread()

#include <time.h> // Timing function
#include <iostream>

#include "process.h" // file under test

using BScanner::ImgProcess;

TEST_CASE( "Image is processed", "[image]" )
{
    // Generate section for X megapixel values
    auto MP = GENERATE(2, 8, 16, 20);

    std::vector<std::string> imageNames;
    std::array<float,3> expectedTimes;

    switch(MP) {
        case 2: // 1080p
            imageNames = { "/home/filip/Repos/Broad/barcode-scanner/images/IMG_20210428_163227.jpg" };
            expectedTimes = { 0.04, 0.06, 0.04 };
            break;
        case 8: // 4K
            imageNames = { "/home/filip/Repos/Broad/barcode-scanner/images/IMG_20210428_163227.jpg" };
            expectedTimes = { 0.04, 0.06, 0.04 };
            break;
        case 16: // 5K
            imageNames = { "/home/filip/Repos/Broad/barcode-scanner/images/IMG_20210428_163227.jpg" };
            expectedTimes = { 0.04, 0.06, 0.04 };
            break;
        case 20: // 6K
            imageNames = { "/home/filip/Repos/Broad/barcode-scanner/images/IMG_20210428_163227.jpg" };
            expectedTimes = { 0.04, 0.06, 0.04 };
            break;
        default: // Default/Nothing
            imageNames = { "/home/filip/Repos/Broad/barcode-scanner/images/IMG_20210428_163227.jpg" };
            expectedTimes = { 0.04, 0.06, 0.04 };
            break;
    }

    SECTION( "XMP Image" )
    {
        BScanner::ImgProcess imgPrc;
        std::vector<cv::Mat> MpImages;

        for(int i = 0; i < imageNames.size(); i++)
        {
            // Read image to Mat
            MpImages.push_back(cv::imread(imageNames[i]));
        }
        
        SECTION( "Far pass" )
        {
            // Start Far timer
            clock_t far = clock();

            // Apply Far pass of Far vector
            for(int i = 0; i < MpImages.size(); i++)
            {
                std::vector<std::tuple<cv::Mat, cv::RotatedRect>> passes;
                imgPrc.ThresholdPass(false, MpImages[i], passes);
            }
            
            // Stop far timer
            clock_t farLog = clock();
            double farTime = (double)(farLog - far) / CLOCKS_PER_SEC;

            // Average time
            auto avgFarTime = farTime / MpImages.size();
            std::cout << "Far time: " << avgFarTime << std::endl;

            REQUIRE(avgFarTime <= expectedTimes[0]);

            SECTION( "Far and Near pass" )
            {
                // Start Near timer
                clock_t near = clock();

                // Apply Near pass
                for(int i = 0; i < MpImages.size(); i++)
                {
                    std::vector<std::tuple<cv::Mat, cv::RotatedRect>> nearPasses;
                    imgPrc.ThresholdPass(true, MpImages[i], nearPasses);
                }

                // Stop Near timer
                clock_t nearLog = clock();
                double nearTime = (double)(nearLog - near) / CLOCKS_PER_SEC;

                // Average Near time
                auto avgNearTime = nearTime / MpImages.size();

                // Sum Far and Near passes
                auto avgTime = avgFarTime + avgNearTime;

                std::cout << "Far + Near time: " << avgTime << std::endl;

                REQUIRE(avgTime <= expectedTimes[1]);
            }
        }

        SECTION( "Near pass" )
        {
            // Start Near timer
            clock_t near = clock();

            // Apply Near pass
            for(int i = 0; i < MpImages.size(); i++)
            {
                std::vector<std::tuple<cv::Mat, cv::RotatedRect>> passes;
                imgPrc.ThresholdPass(true, MpImages[i], passes);
            }

            // Stop Near timer
            clock_t nearLog = clock();
            double nearTime = (double)(nearLog - near) / CLOCKS_PER_SEC;

            // Average Near time
            auto avgNearTime = nearTime / MpImages.size();

            std::cout << "Near time: " << avgNearTime << std::endl;

            REQUIRE(avgNearTime <= expectedTimes[2]);
        }
    }
}