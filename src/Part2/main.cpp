// Lap01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
#include <opencv2/imgproc.hpp>
#include "GeometricTransformer.h"
#include<iostream>
using namespace std;

int main(int argc, const char** argv)
{
    // Read input image
    GeometricTransformer geometricTransformer;
    PixelInterpolate* pixelInterpolate;
    Mat desImg;

    /*
    pixelInterpolate = new NearestNeighborInterpolate();
    Mat srcImag = imread("N:\\2.png", cv::IMREAD_UNCHANGED);
    geometricTransformer.Scale(srcImag, desImg, 2, 2, pixelInterpolate);
    cv::imwrite("N:\\output.png", desImg);
    */

    if (argc < 5 || argc > 7) {
        cerr << "Argument parameter's error!" << endl;
        return 1;
    }

    // Interpolate 
    if (strcmp(argv[2], "-nn") == 0) {
        pixelInterpolate = new NearestNeighborInterpolate();
    }
    else if (strcmp(argv[2], "-bl") == 0) {
        pixelInterpolate = new BilinearInterpolate();
    }
    else {
        cerr << "Error!!! Interpolate : " << argv[2] << " is not supported." << endl;
        return 1;
    }

    if (strcmp(argv[1], "-zoom") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[5], cv::IMREAD_UNCHANGED);
        stringstream s1(argv[3]), s2(argv[4]);
        float sx, sy;
        s1 >> sx; s2 >> sy;

        if (geometricTransformer.Scale(srcImg, desImg, sx, sy, pixelInterpolate) == 1) {
            cerr << "Error!!! Failed to zoom in/out image." << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[6], desImg);
    }
    else if (strcmp(argv[1], "-resize") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[5], cv::IMREAD_UNCHANGED);
        stringstream s1(argv[3]), s2(argv[4]);
        float nw, nh;
        s1 >> nw; s2 >> nh;

        if (geometricTransformer.Resize(srcImg, desImg, nw, nh, pixelInterpolate) == 1) {
            cerr << "Error!!! Failed to resize image." << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[6], desImg);
    }
    else if (strcmp(argv[1], "-rotK") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[4], cv::IMREAD_UNCHANGED);
        stringstream s1(argv[3]);
        float angle;
        s1 >> angle;

        if (geometricTransformer.RotateUnkeepImage(srcImg, desImg, angle, pixelInterpolate) == 1) {
            cerr << "Error!!! Failed to rotate image (crop)." << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[5], desImg);
    }
    else if (strcmp(argv[1], "-rotP") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[4], cv::IMREAD_UNCHANGED);
        stringstream s1(argv[3]);
        float angle;
        s1 >> angle;

        if (geometricTransformer.RotateKeepImage(srcImg, desImg, angle, pixelInterpolate) == 1) {
            cerr << "Error!!! Failed to rotate image (keep the whole image)." << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[5], desImg);
    }
    else if (strcmp(argv[1], "-flipV") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[3], cv::IMREAD_UNCHANGED);

        if (geometricTransformer.Flip(srcImg, desImg, 0, pixelInterpolate) == 1) {
            cerr << "Error!!! Failed to flip image vertically" << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[4], desImg);
    }
    else if (strcmp(argv[1], "-flipH") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[3], cv::IMREAD_UNCHANGED);

        if (geometricTransformer.Flip(srcImg, desImg, 1, pixelInterpolate) == 1) {
            cerr << "Error!!! Failed to flip image horizontally" << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[4], desImg);
    }
    else
    {
        cerr << "Request does not exist: "<< argv[1] << endl;
        delete pixelInterpolate;
        return 1;
    }

    delete pixelInterpolate;
    return 0;
}


