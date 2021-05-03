#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <vector>

using namespace cv;

int main(int argc, char* argv[])
{
    cv::Mat img = cv::imread(argv[3]);

    // Convert RGB Mat to GRAY
    cv::Mat grad;
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // Calculate image gradient
    const int ddepth = CV_32F;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    // Blur for cleaner image
    cv::GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);

    // nth image derivative w/ Sobel operator
    /*
    cv::Sobel(gray, grad_x, ddepth, 1, 0, -1);
    cv::Sobel(gray, grad_y, ddepth, 0, 1, -1);
    */

    // 1st image derivative w/ Scharr op
    Scharr(img, grad_x, ddepth, 1, 0);
    Scharr(img, grad_y, ddepth, 0, 1);

    // converting back to CV_8U
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    // Display gradiented img
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    const String namedWindow = "Sobel derived gradient";
    cv::namedWindow(namedWindow, WINDOW_NORMAL);
    cv::imshow(namedWindow, grad);

    cv::waitKey(0);

    return 0;
}