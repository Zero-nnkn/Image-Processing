// Lab03.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Blur.h"
#include "EdgeDetector.h"

int main(int argc, const char** argv)
{
    // Read input image
    Blur blur;
    EdgeDetector edgeDetector;
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

        if (blur.BlurImage(srcImg, desImg, kx, ky, 0) == 1) {
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

        if (blur.BlurImage(srcImg, desImg, kx, ky, 1) == 1) {
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

        if (blur.BlurImage(srcImg, desImg, kx, ky, 2) == 1) {
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

        if (edgeDetector.DetectEdge(srcImg, desImg, 0) == 1) {
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

        if (edgeDetector.DetectEdge(srcImg, desImg, 1) == 1) {
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

        if (edgeDetector.DetectEdge(srcImg, desImg, 2) == 1) {
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


    return 0;
}

