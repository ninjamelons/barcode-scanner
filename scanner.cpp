#include "scanner.h"

using BScanner::Scanner;

cv::Mat Scanner::processRaw(const cv::Mat& frame, BScanner::Threshold thresh)
{
    cv::Mat rotated, output;

    cv::RotatedRect rect = processor.FarThresholdPass(frame, thresh);
    rotated = processor.TransformRotated(frame, rect);
    output = processor.PrepareRegion(frame);

    return output;
}

BScanner::Result Scanner::decodeRaw(const cv::Mat& frame)
{
    // Blur, threshold, kernelx, kernely, erode, dilate
    BScanner::Threshold thresh = BScanner::Threshold(13, 130, 13, 13, 4, 4);
    
    cv::Mat output = processRaw(frame, thresh);

    return decodeProcessed(output);
}

BScanner::Result Scanner::decodeProcessed(const cv::Mat& processed)
{
    return decoder.decode(processed);
}