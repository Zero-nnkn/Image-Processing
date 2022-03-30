// Lab03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Blur.h"
#include "EdgeDetector.h"
#include "Canny.h"

int main(int argc, const char** argv)
{
    /*
    // Read input image
    Blur blur;
    Mat desImg;

    if (argc < 4 || argc > 6) {
        cerr << "Argument parameter's error!" << endl;
        return 1;
    }

    if (strcmp(argv[1], "-avg") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[4], cv::IMREAD_GRAYSCALE);
        stringstream s1(argv[2]), s2(argv[3]);
        int kx, ky;
        s1 >> kx; s2 >> ky;

        if (Blur::BlurImage(srcImg, desImg, kx, ky, Blur::AVERAGE_FLAG) == 1) {
            cerr << "Error!!! Failed to blur this image using average filtering" << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[5], desImg);
    }
    else if (strcmp(argv[1], "-med") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[4], cv::IMREAD_GRAYSCALE);
        stringstream s1(argv[2]), s2(argv[3]);
        int kx, ky;
        s1 >> kx; s2 >> ky;

        if (Blur::BlurImage(srcImg, desImg, kx, ky, Blur::MEDIAN_FLAG) == 1) {
            cerr << "Error!!! Failed to blur this image using median filtering" << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[5], desImg);
    }
    else if (strcmp(argv[1], "-gau") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[4], cv::IMREAD_GRAYSCALE);
        stringstream s1(argv[2]), s2(argv[3]);
        int kx, ky;
        s1 >> kx; s2 >> ky;

        if (Blur::BlurImage(srcImg, desImg, kx, ky, Blur::GAUSSIAN_FLAG) == 1) {
            cerr << "Error!!! Failed to blur this image using Gaussian filtering" << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[5], desImg);
    }
    else if (strcmp(argv[1], "-sobel") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[2], cv::IMREAD_GRAYSCALE);

        if (EdgeDetector::DetectEdge(srcImg, desImg, EdgeDetector::SOBEL_FLAG) == 1) {
            cerr << "Error!!! Failed to detect edges of this image using Sobel Operator" << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[3], desImg);
    }
    else if (strcmp(argv[1], "-prew") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[2], cv::IMREAD_GRAYSCALE);

        if (EdgeDetector::DetectEdge(srcImg, desImg, EdgeDetector::PREWITT_FLAG) == 1) {
            cerr << "Error!!! Failed to detect edges of this image using Prewitt Operator" << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[3], desImg);
    }
    else if (strcmp(argv[1], "-lap") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[2], cv::IMREAD_GRAYSCALE);

        if (EdgeDetector::DetectEdge(srcImg, desImg, EdgeDetector::LAPLACE_FLAG) == 1) {
            cerr << "Error!!! Failed to detect edges of this image using Laplace Operator" << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[3], desImg);
    }
    else
    {
        cerr << "Request does not exist: " << argv[1] << endl;
        return 1;
    }
    */

    Mat src = imread("test2.png", IMREAD_GRAYSCALE);
    Mat edge;
    Mat dst = src.clone();
    //Blur::BlurImage(src, dst, 5, 5, Blur::AVERAGE_FLAG);
    //imshow("a", dst);
    //waitKey(0);
    //EdgeDetector::DetectEdge(src, dst, EdgeDetector::SOBEL_FLAG);
    Canny::DetectEdge(src, dst, 30, 200);
    imshow("MyCanny", dst);
    waitKey(0);
    destroyAllWindows();

    cv::blur(src, dst, cv::Size(5, 5));
    cv::Canny(dst, dst, 30, 200);
    imshow("OpencvCanny", dst);
    waitKey(0);
    destroyAllWindows();

    return 0;
}

