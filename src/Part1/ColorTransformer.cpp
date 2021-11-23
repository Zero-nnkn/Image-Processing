#include "ColorTransformer.h"
#include<iostream>
using namespace std;

ColorTransformer::ColorTransformer() {}
ColorTransformer::~ColorTransformer() {}

int ColorTransformer::SqueezeValue(int v)
{
    return std::max(0, std::min(v, 255));
}


void ColorTransformer::scaleHistMatrix(Mat& histMatrix) // Do not solve channel 4
{
    int maxValue = this->getMax(histMatrix);
    double scale = (maxValue * 1.0) / (HISTOGRAM_HEIGHT - DELTA);

    int* pHistData = (int*)histMatrix.data;
    int histChannels = histMatrix.channels();
    int channel = std::min(histMatrix.rows, 3);
    int histWidthStep = NUM_COLOR;

    for (int i = 0; i < NUM_COLOR; i++) {
        for (int k = 0; k < channel; k++) {
            int* temp = pHistData + k * histWidthStep + i * histChannels;
            temp[0] = (int)temp[0] / scale;
        }
    }
}

int ColorTransformer::getMax(const Mat& histMatrix) // Do not solve channel 4
{
    int* pHistData = (int*)histMatrix.data;
    int histChannels = histMatrix.channels();
    int channel = std::min(histMatrix.rows, 3);
    int histWidthStep = NUM_COLOR;

    int max[3] = { 0 };
    for (int i = 0; i < NUM_COLOR; i++) {
        for (int k = 0; k < channel; k++) {
            if ((pHistData + k * histWidthStep + i * histChannels)[0] > max[k])
                max[k] = (pHistData + k * histWidthStep + i * histChannels)[0];
        }
    }

    return std::max(std::max(max[0], max[1]), max[2]);
}


void ColorTransformer::ChangeBrighnessChannel(const Mat& sourceImage, Mat& destinationImage, short b, int channel)
{
    int width = sourceImage.cols;
    int height = sourceImage.rows;

    //Number of channels
    int srcChannels = sourceImage.channels();
    int desChannels = destinationImage.channels();

    // Distance between 2 pixel (bytes) in a column
    int srcWidthStep = (int)sourceImage.step[0];
    int desWidthStep = (int)destinationImage.step[0];

    uchar* pSrcData = (uchar*)sourceImage.data;
    uchar* pDesData = (uchar*)destinationImage.data;
    for (int i = 0; i < height; i++, pSrcData += srcWidthStep, pDesData += desWidthStep)
    {
        uchar* pSrcRow = pSrcData;
        uchar* pDesRow = pDesData;
        for (int j = 0; j < width; j++, pSrcRow += srcChannels, pDesRow += desChannels)
        {
            pDesRow[channel] = this->SqueezeValue((int)(pSrcRow[channel] + b));
        }
    }
}

int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b)
{
    if (sourceImage.data == NULL)
        return 1;

    destinationImage = sourceImage.clone();
    int desChannels = destinationImage.channels();
    if (desChannels != 1 && desChannels != 3 && desChannels != 4)
        return 1;


    int channel = min(desChannels, 3); //Do not change channel 4
    for (int i = 0; i < channel; i++)
    {
        this->ChangeBrighnessChannel(sourceImage, destinationImage, b, i);
    }
    return 0;
}


void ColorTransformer::ChangeContrastChannel(const Mat& sourceImage, Mat& destinationImage, double c, int channel)
{
    int width = sourceImage.cols;
    int height = sourceImage.rows;

    //Number of channels
    int srcChannels = sourceImage.channels();
    int desChannels = destinationImage.channels();

    // Distance between 2 pixel (bytes) in a column
    int srcWidthStep = (int)sourceImage.step[0];
    int desWidthStep = (int)destinationImage.step[0];

    uchar* pSrcData = (uchar*)sourceImage.data;
    uchar* pDesData = (uchar*)destinationImage.data;
    for (int i = 0; i < height; i++, pSrcData += srcWidthStep, pDesData += desWidthStep)
    {
        uchar* pSrcRow = pSrcData;
        uchar* pDesRow = pDesData;
        for (int j = 0; j < width; j++, pSrcRow += srcChannels, pDesRow += desChannels)
        {
            pDesRow[channel] = this->SqueezeValue((int)(pSrcRow[channel] * c));
        }
    }
}

int ColorTransformer::ChangeContrast(const Mat& sourceImage, Mat& destinationImage, double c)
{
    if (sourceImage.data == NULL)
        return 1;

    destinationImage = sourceImage.clone();
    int desChannels = destinationImage.channels();
    if (desChannels != 1 && desChannels != 3 && desChannels != 4)
        return 1;

    int channel = min(desChannels, 3); //Do not change channel 4
    for (int i = 0; i < channel; i++)
    {
        this->ChangeContrastChannel(sourceImage, destinationImage, c, i);
    }
    return 0;
}

void ColorTransformer::CalcHistogramChannel(const Mat& sourceImage, Mat& histMatrix, int channel)
{
    int width = sourceImage.cols;
    int height = sourceImage.rows;

    //Number of channels
    int srcChannels = sourceImage.channels();
    int histChannels = histMatrix.channels();

    // Distance between 2 pixel (bytes) in a column 
    int srcWidthStep = (int)sourceImage.step[0];
    int histWidthStep = NUM_COLOR;

    uchar* pSrcData = (uchar*)sourceImage.data;
    int* pHistData = (int*)histMatrix.data;
    for (int i = 0; i < height; i++, pSrcData += srcWidthStep)
    {
        uchar* pSrcRow = pSrcData;
        for (int j = 0; j < width; j++, pSrcRow += srcChannels)
        {
            int value = (int)pSrcRow[channel];
            (pHistData + channel * histWidthStep + value * histChannels)[0] += 1;
        }
    }
}

int ColorTransformer::CalcHistogram(const Mat& sourceImage, Mat& histMatrix)
{
    if (sourceImage.data == NULL)
        return 1;

    //Number of channels
    int srcChannels = sourceImage.channels();
    histMatrix = cv::Mat::zeros(srcChannels, NUM_COLOR, CV_32SC1);

    int channel = std::min(srcChannels, 3); //Do not change channel 4
    for (int i = 0; i < channel; i++)
    {
        this->CalcHistogramChannel(sourceImage, histMatrix, i);
    }

    return 0;
}


int ColorTransformer::HistogramEqualization(const Mat& sourceImage, Mat& destinationImage)
{
    if (sourceImage.data == NULL)
        return 1;

    cv::Mat histMatrix;
    if (this->CalcHistogram(sourceImage, histMatrix) == 1)
        return 1;

    int width = sourceImage.cols;
    int height = sourceImage.rows;
    destinationImage = sourceImage.clone();
    int total = width * height;

    //Number of channels
    int srcChannels = sourceImage.channels();

    int channel = min(srcChannels, 3); //Do not change channel 4
    int desChannels = destinationImage.channels();
    int histChannels = histMatrix.channels();

    // Distance between 2 pixel (bytes) in a column 
    int srcWidthStep = (int)sourceImage.step[0];
    int desWidthStep = (int)destinationImage.step[0];
    int histWidthStep = NUM_COLOR;


    // Find Cumulative grayscale histogram
    int* pHistData = (int*)histMatrix.data;
    for (int k = 0; k < channel; k++) {
        for (int i = 1; i < NUM_COLOR; i++) {
            (pHistData + k * histWidthStep + i * histChannels)[0]
                += (pHistData + k * histWidthStep + (i - 1) * histChannels)[0];
        }
    }

    // Computer scalse
    int min[3] = { 0 };
    for (int k = 0; k < channel; k++) {
        for (int i = 0; i < NUM_COLOR; i++) {
            if ((pHistData + k * histWidthStep + i * histChannels)[0] != 0) {
                min[k] = (pHistData + k * histWidthStep + i * histChannels)[0];
                break;
            }
        }
    }

    double scale[3] = { 0 };
    for (int k = 0; k < channel; k++) {
        scale[k] = (NUM_COLOR - 1.0) / (total - min[k]);
    }

    // Normalize to [0, NUM_COLOR-1]
    for (int i = 1; i < NUM_COLOR; i++) {
        int* temp = pHistData + i * histChannels;
        for (int k = 0; k < channel; k++)
        {
            (temp + k * histWidthStep)[0] = this->SqueezeValue((int)round(((temp + k * histWidthStep)[0] - min[k]) * scale[k]));
        }
    }

    // Create a result image
    uchar* pSrcData = (uchar*)sourceImage.data;
    uchar* pDesData = (uchar*)destinationImage.data;
    for (int i = 0; i < height; i++, pSrcData += srcWidthStep, pDesData += desWidthStep)
    {
        uchar* pSrcRow = pSrcData;
        uchar* pDesRow = pDesData;
        for (int j = 0; j < width; j++, pSrcRow += srcChannels, pDesRow += desChannels)
        {
            for (int k = 0; k < channel; k++) {
                int value = (int)pSrcRow[k];
                pDesRow[k] = (pHistData + k * histWidthStep + value * histChannels)[0];
            }
        }
    }


    return 0;
}

int ColorTransformer::DrawHistogram(const Mat& histMatrix, Mat& histImage) {
    if (histMatrix.data == NULL)
        return 1;

    cv::Mat scaleHistMatrix = histMatrix.clone();
    this->scaleHistMatrix(scaleHistMatrix);
    int channel = std::min(scaleHistMatrix.rows, 3); // Do not change channel 4

    int width = HISTOGRAM_WIDTH;
    int height = HISTOGRAM_HEIGHT;
    histImage = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    int histImageChannels = histImage.channels();
    int histImageWidthStep = (int)histImage.step[0];


    int* pHistData = (int*)scaleHistMatrix.data;
    int histChannels = scaleHistMatrix.channels();
    int histWidthStep = NUM_COLOR;

    // Draw 
    if (channel == 1) { //1 channel
        int THICKNESS = 3;
        for (int i = 0; i < NUM_COLOR; i++) {
            int* tempHistMatrix = pHistData + i * histChannels;
            int value = tempHistMatrix[0];
            for (int h = height - value; h < height; h++) {
                uchar* pHistImageData = (uchar*)histImage.data + h * histImageWidthStep;
                for (int w = i * THICKNESS; w < (i + 1) * THICKNESS; w++) {
                    // Draw color
                    uchar* pHistImageRow = pHistImageData + w * histImageChannels;
                    pHistImageRow[0] = 169;
                    pHistImageRow[1] = 169;
                    pHistImageRow[2] = 169;
                }
            }
        }
    }
    else { //3 channel
        int THICKNESS = 1;
        for (int k = channel - 1; k >= 0; k--) { // R -> G -> B
            for (int i = 0; i < NUM_COLOR; i++) {
                int* tempHistMatrix = pHistData + k * histWidthStep + i * histChannels;
                int value = tempHistMatrix[0];

                for (int h = height - value; h < height; h++) {
                    uchar* pHistImageData = (uchar*)histImage.data + h * histImageWidthStep;
                    for (int w = (channel - k - 1) * NUM_COLOR * THICKNESS + i * THICKNESS; w < (channel - k - 1) * NUM_COLOR * THICKNESS + (i + 1) * THICKNESS; w++) {
                        // Draw color
                        uchar* pHistImageRow = pHistImageData + w * histImageChannels;
                        for (int c = 0; c < channel; c++)
                            if (c != k)
                                pHistImageRow[c] = (uchar)0;
                    }
                }
            }
        }
    }

    return 0;
}
