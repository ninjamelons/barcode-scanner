#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp> // test case
#include <opencv2/core.hpp> // core / cv scope
#include <opencv2/imgcodecs.hpp> // imread()

#include "decoder.h"
#include "ean13.h"
#include "process.h"

TEST_CASE( "Prepare and decode image", "[decode]" )
{
    // Generate section for X megapixel values
    auto MP = GENERATE("/home/filip/Repos/Broad/barcode-scanner/images/Barcode_Close.jpg",
                        "/home/filip/Repos/Broad/barcode-scanner/images/Barcode_Medium.jpg",
                        "/home/filip/Repos/Broad/barcode-scanner/images/Barcode_Far.jpg");

    BScanner::ImgProcess imgProc;
    BScanner::Decoder* decoder = new BScanner::Ean13();

    cv::Mat in, out;

    in = cv::imread(MP);
    out = imgProc.PrepareRegion(in);

    BENCHMARK( "Benchmark decode" ) {
        return decoder->decode(out);
    };

    BScanner::Result res = decoder->decode(out);

    REQUIRE(res.value == "8806090168376");
}