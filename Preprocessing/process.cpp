#include "process.h"

using BScanner::ImgProcess;

ImgProcess::ImgProcess() {}

void ImgProcess::ThresholdPass(bool isNear, const cv::Mat& imgIn,
    std::vector<std::tuple<cv::Mat, cv::RotatedRect>>& passes, Threshold thresh)
{
    cv::Mat input = imgIn;
    if( isNear )
    {
        cv::RotatedRect rect = NearThresholdPass(input);
        passes.push_back(std::make_tuple(imgIn(rect.boundingRect()), rect));
    } else
    {
        cv::RotatedRect t1 = FarThresholdPass(input, thresh, 1920);
        
        cv::Mat patch;

        try
        {
            patch = imgIn(t1.boundingRect());
        }
        catch (cv::Exception exc)
        {
            patch = imgIn;
        }
        passes.push_back(std::make_tuple(patch, t1));
    }
}

void ImgProcess::GradientPass(const int angle)
{
    std::cout << "Gradient Pass" << std::endl;
}

cv::RotatedRect ImgProcess::FarThresholdPass(const cv::Mat& imgIn, Threshold thresh, int maxDim)
{
    cv::Mat imgOut = imgIn.clone();
    float aspectTransform = 1;

    // Get largest dimension & Resize to maxDim
    if( imgIn.size().aspectRatio() >= 1 ) //aspect = (width/height)
    {
        // Width largest OR Equal
        if( !(imgIn.size().width <= maxDim) )
        {
            aspectTransform = imgIn.cols / maxDim;
        }
    }
    else
    {
        // Height largest
        if( !(imgIn.size().height <= maxDim) )
        {
            aspectTransform = imgIn.rows / maxDim;
        }
    }

    // Resize w/ aspect
    // Resize FIRST - all other operations are highly taxing
    cv::resize(imgOut, imgOut, cv::Size(), 1/aspectTransform, 1/aspectTransform);

    // Convert RGB Mat to GRAY
    cv::cvtColor(imgOut, imgOut, cv::COLOR_BGR2GRAY);

    // Process resized image
    const int ddepth = CV_32F;
    cv::Mat grad_x, grad_y, abs_grad_x, abs_grad_y;

    // Blur for cleaner image
    cv::GaussianBlur(imgOut, imgOut, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
    
    // nth image derivative w/ Sobel operator
    cv::Sobel(imgOut, grad_x, ddepth, 1, 0, -1);
    cv::Sobel(imgOut, grad_y, ddepth, 0, 1, -1);    

    // converting back to CV_8U
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    // Display gradiented img
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, imgOut);

    unsigned int blur = thresh.GetBlur();
    // Blur to prepare for threshold
    cv::blur(imgOut, imgOut, cv::Size(blur, blur));

    // Threshold gradient image
    cv::threshold(imgOut, imgOut, thresh.GetThreshold(), 255, cv::THRESH_BINARY);

    // Fill larger gaps between lines
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(21, 7));
    cv::morphologyEx(imgOut, imgOut, cv::MORPH_CLOSE, kernel);
    
    // Remove noisy artifacting & Enlarge main focus
    cv::erode(imgOut, imgOut, kernel, cv::Point(-1,-1), 3.5, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
    cv::dilate(imgOut, imgOut, kernel, cv::Point(-1,-1), 18, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());

    // Get largest contour
    std::vector<std::vector<cv::Point>> imgContours;
    cv::findContours( imgOut, imgContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );
    std::sort(imgContours.begin(), imgContours.end(),
        [](std::vector<cv::Point> i, std::vector<cv::Point> j) -> bool {
            return cv::contourArea(i) > cv::contourArea(j);
        }
    );

    // Crop img to ROI
    cv::Mat3b patch;
    cv::Point2f vtx[4];
    cv::RotatedRect rect;
    if(!imgContours.empty()) {
        // Scale RotatedRect to Raw size
        rect = cv::minAreaRect(imgContours[0]);
        rect.center.x *= aspectTransform;
        rect.center.y *= aspectTransform;
        rect.size.width *= aspectTransform;
        rect.size.height *= aspectTransform;
    }

    cv::namedWindow("Process imgOut", cv::WINDOW_NORMAL);
    cv::imshow("Process imgOut", imgOut);

    return rect;
}

cv::RotatedRect ImgProcess::NearThresholdPass(const cv::Mat& imgIn)
{
    cv::RotatedRect rect;
    return rect;
}