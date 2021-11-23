#include"Converter.h"

Converter::Converter() {}
Converter::~Converter() {}

int Converter::RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage)
{
    if (sourceImage.data == NULL)
        return 1;

    int width = sourceImage.cols;
    int height = sourceImage.rows;
    destinationImage = cv::Mat(height, width, CV_8UC1);

    //Number of channels
    int srcChannels = sourceImage.channels();
    int desChannels = destinationImage.channels();
    if (srcChannels != 3 && srcChannels != 4)
        return 1;

    // Distance between 2 pixel (bytes) in a column
    int srcWidthStep = (int)sourceImage.step[0];
    int desWidthStep = (int)destinationImage.step[0];

    // RGB -> GrayScale
    uchar* pSrcData = (uchar*)sourceImage.data;
    uchar* pDesData = (uchar*)destinationImage.data;
    for (int i = 0; i < height; i++, pSrcData += srcWidthStep, pDesData += desWidthStep)
    {
        uchar* pSrcRow = pSrcData;
        uchar* pDesRow = pDesData;
        for (int j = 0; j < width; j++, pSrcRow += srcChannels, pDesRow += desChannels)
        {
            uchar B = pSrcRow[0];
            uchar G = pSrcRow[1];
            uchar R = pSrcRow[2];
            uchar Gray = (uchar)(R * 0.299 + G * 0.587 + 0.114 * B);
            pDesRow[0] = Gray;
        }
    }
    return 0;
}


int Converter::RGB2HSV(const Mat& sourceImage, Mat& destinationImage)
{
    if (sourceImage.data == NULL)
        return 1;

    int width = sourceImage.cols;
    int height = sourceImage.rows;
    destinationImage = cv::Mat(height, width, CV_8UC3);

    //Number of channels
    int srcChannels = sourceImage.channels();
    int desChannels = destinationImage.channels();
    if (srcChannels != 3 && srcChannels != 4)
        return 1;

    // Distance between 2 pixel (bytes) in a column
    int srcWidthStep = (int)sourceImage.step[0];
    int desWidthStep = (int)destinationImage.step[0];

    // RGB -> GrayScale
    uchar* pSrcData = (uchar*)sourceImage.data;
    uchar* pDesData = (uchar*)destinationImage.data;
    for (int i = 0; i < height; i++, pSrcData += srcWidthStep, pDesData += desWidthStep)
    {
        uchar* pSrcRow = pSrcData;
        uchar* pDesRow = pDesData;
        for (int j = 0; j < width; j++, pSrcRow += srcChannels, pDesRow += desChannels)
        {
            double B = pSrcRow[0] / 255.0;
            double G = pSrcRow[1] / 255.0;
            double R = pSrcRow[2] / 255.0;

            double H, S, V;
            double min = std::min(std::min(R, G), B);
            double max = std::max(std::max(R, G), B);
            double delta = max - min;

            // V
            V = max;

            // S
            if (V != 0)
                S = delta / V;
            else {
                S = 0;
                H = 0;
                pDesRow[0] = H;
                pDesRow[1] = S;
                pDesRow[2] = V;
                continue;
            }

            // H
            if (delta <= 0)
                H = 0;
            else if (R >= V)
                H = 60.0 * (G - B) / delta;
            else if (G >= V)
                H = 120 + 60.0 * (B - R) / delta;
            else if (B >= V)
                H = 240.0 + 60.0 * (R - G) / delta;
            else
                H = 0;

            if (H < 0.0)
                H += 360;

            pDesRow[0] = H / 2;
            pDesRow[1] = S * 255;
            pDesRow[2] = V * 255;
        }
    }
    return 0;
}


int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type)
{
    if (type == 0)
    {
        //RGB -> Gray
        if (this->RGB2GrayScale(sourceImage, destinationImage) == 1)
            return 1;
    }
    else if (type == 1) {
        // RGV -> HSV
        if (this->RGB2HSV(sourceImage, destinationImage) == 1)
            return 1;
    }
    else
        return 1;

    return 0;
}
