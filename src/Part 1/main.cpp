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
#include "Converter.h"
#include "ColorTransformer.h"
#include <opencv2/imgproc.hpp>
#include<iostream>
using namespace std;

int main(int argc, const char** argv)
{
    Converter converter;
    ColorTransformer colorTransformer;

    // Read input image
    Mat desImg;
    Mat histMatrix;


    if (strcmp(argv[1], "-rgb2gray") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[2], cv::IMREAD_UNCHANGED);
        // Convert RGB to Grayscale
        if (converter.Convert(srcImg, desImg, 0) == 1) {
            cerr << "Error!!! Image cannot be converted to Grayscale. Check your input image." << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[3], desImg);
    }
    else if (strcmp(argv[1], "-rgb2hsv") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[2], cv::IMREAD_UNCHANGED);
        // Convert RGB to HSV
        if (converter.Convert(srcImg, desImg, 1) == 1) {
            cerr << "Error!!! Image cannot be converted to HSV. Check your input image." << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[3], desImg);
    }
    else if (strcmp(argv[1], "-bright") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[3], cv::IMREAD_UNCHANGED);
        // Change brighness 
        stringstream s(argv[2]);
        short b;
        s >> b;
        if (colorTransformer.ChangeBrighness(srcImg, desImg, b) == 1) {
            cerr << "Error!!! Image brightness cannot be changed. Check the input parameters again." << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[4], desImg);
    }
    else if (strcmp(argv[1], "-contrast") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[3], cv::IMREAD_UNCHANGED);
        // Change contrast
        stringstream s(argv[2]);
        double c;
        s >> c;
        if (colorTransformer.ChangeContrast(srcImg, desImg, c) == 1) {
            cerr << "Error!!! Image contrast cannot be changed. Check the input parameters again." << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[4], desImg);
    }
    else if (strcmp(argv[1], "-drawhist") == 0)
    {
        // Read input image
        Mat srcImg = imread(argv[2], cv::IMREAD_UNCHANGED);
        // Calculate histogram
        if (colorTransformer.CalcHistogram(srcImg, histMatrix) == 1) {
            cerr << "Error!!! The histogram of the image cannot be calculated. Check the input parameters again." << endl;
            return 1;
        }
        // Draw histogram
        if (colorTransformer.DrawHistogram(histMatrix, desImg) == 1) {
            cerr << "Error!!! The histogram of the image cannot be drawn. Check the input parameters again." << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[3], desImg);
    }
    else if (strcmp(argv[1], "equalhist"))
    {
        // Read input image
        Mat srcImg = imread(argv[2], cv::IMREAD_UNCHANGED);
        // Equalize histogram
        if (colorTransformer.HistogramEqualization(srcImg, desImg) == 1) {
            cerr << "Error!!! Histogram equalization fails. Check the input parameters again." << endl;
            return 1;
        }

        // Save output
        cv::imwrite(argv[3], desImg);
    }
    else
    {
        cerr << "Request does not exist" << endl;
        return 1;
    }

    return 0;
}


