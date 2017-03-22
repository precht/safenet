#include "CMT.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>


int main(int argc, char **argv)
{
    cv::Mat img1(200, 300, CV_8UC3);

    int val = 0;
    const int chls = img1.channels();

    for (int i = 0; i < img1.rows; i++)
        for (int j = 0; j < img1.cols; j++)
            for (int k = 0; k < chls; k++, val += 51)
                img1.at<char>(i, j * chls + k) = val % 256;

    cv::imwrite("sample.png", img1);
    cv::Mat img2 = cv::imread("sample.png");

    if (img2.empty())
    {
        std::cout << "Failed to load image." << std::endl;
    }
    else
    {
        cv::namedWindow("window");
        cv::imshow("window", img2);
        cv::waitKey(0);
        cv::destroyAllWindows();

        std::cout << "Success!" << std::endl;
    }

    return 0;
}
