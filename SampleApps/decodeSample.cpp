#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdarg.h>

#include "threshold.h"
#include "process.h"
#include "decoder.h"
#include "code39.h"
#include "ean13.h"

using namespace cv;

int dim_slider, threshold_slider, blur_slider;

struct ImageMats {
    std::vector<cv::Mat> matIn;
    BScanner::Threshold thresh;
};

BScanner::Result decodeImg(const Mat& img);

void On_Trackbar( int, void* ptr )
{
    struct ImageMats *mats = (struct ImageMats *)ptr;
    BScanner::ImgProcess imgPrc;
    mats->thresh.SetBlur((int) blur_slider);
    mats->thresh.SetThreshold((int) threshold_slider);

    cv::Mat img_close_thresh, img_med_thresh, img_far_thresh;

    // Process imgs here
    img_close_thresh = imgPrc.PrepareRegion(mats->matIn[0], dim_slider);
    img_med_thresh = imgPrc.PrepareRegion(mats->matIn[1], dim_slider);
    //img_far_thresh = imgPrc.PrepareRegion(mats->matIn[2], dim_slider);

    // Decode img here
    auto res1 = decodeImg(img_close_thresh);
    auto res2 = decodeImg(img_med_thresh);
    //decodeImg(img_far_thresh);


    for (int i = 0; i < 4; i++)
    {
        line(img_close_thresh, res1.coords[i], res1.coords[(i+1)%4], cv::Scalar(0,255,0), 1, cv::LINE_AA);
        line(img_med_thresh, res2.coords[i], res2.coords[(i+1)%4], cv::Scalar(0,255,0), 1, cv::LINE_AA);
    }
    
    cv::imshow("Close Proc", img_close_thresh);
    cv::imshow("Med Proc", img_med_thresh);
    //cv::imshow("Far Proc", img_far_thresh);
}

int main(int argc, char* argv[])
{
    // Processed Windows
    cv::namedWindow("Close Proc", cv::WINDOW_NORMAL);
    cv::namedWindow("Med Proc", cv::WINDOW_NORMAL);
    //cv::namedWindow("Far Proc", cv::WINDOW_NORMAL);

    ImageMats mats;
    // Load barcodes where distance to camera is ...
    mats.matIn.push_back(cv::imread(argv[1]));
    mats.matIn.push_back(cv::imread(argv[2]));
    //mats.matIn.push_back(cv::imread(argv[3]));
    mats.thresh = BScanner::Threshold();

    //Process image (Minor thresholding and blur for easier/cleaner decode)
    int dim_max, threshold_max, blur_max;

    dim_max = 500;
    threshold_max = 255;
    blur_max = 31;

    dim_slider = 300;
    threshold_slider = 110;
    blur_slider = 1;

    mats.thresh.SetBlur((int) blur_slider);
    mats.thresh.SetThreshold((int) threshold_slider);

    cv::namedWindow("Trackbars", cv::WINDOW_NORMAL);
    cv::createTrackbar( "Dimension", "Trackbars", &dim_slider, dim_max, On_Trackbar, (void*)&mats );
    cv::createTrackbar( "Threshold", "Trackbars", &threshold_slider, threshold_max, On_Trackbar, (void*)&mats );
    cv::createTrackbar( "Blur", "Trackbars", &blur_slider, blur_max, On_Trackbar, (void*)&mats );

    On_Trackbar( 0,(void*)&mats );

    cv::waitKey(0);

    return 0;
}

BScanner::Result decodeImg(const cv::Mat& img)
{
    BScanner::Decoder* decoder = new BScanner::Ean13();
    BScanner::Result res = decoder->decode(img);

    // Print decoded value & coordinates
    std::cout << res.symbology <<": "<< res.value << "; ";
    if(res.coords.size() > 0)
    {
        std::cout << "Coords: ["<<
            res.coords[0] <<','<< res.coords[1] <<','<< res.coords[2] <<','<< res.coords[3] << std::endl;
    }

    delete decoder;
    return res;
}