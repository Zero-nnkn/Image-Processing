#include "Blur.h"

Blur::Blur(){}
Blur::~Blur(){}

int Blur::BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method) {
	if (sourceImage.empty() || sourceImage.channels() != 1)
		return 1;

	switch (method)
	{
	case 0:
	{
		if (this->AverageOpt(sourceImage, destinationImage, kWidth, kHeight) == 1)
			return 1;
		break;
	}
	case 1:
	{
		if (this->MedianOpt(sourceImage, destinationImage, kWidth, kHeight) == 1)
			return 1;
		break;
	}
	case 2:
	{
		if (this->GaussianOpt(sourceImage, destinationImage, kWidth, kHeight) == 1)
			return 1;
		break;
	}
	default: {
		return 1;
		break;
	}
	}

	return 0;
}


int Blur::AverageOpt(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight)
{
	int len = kWidth * kHeight;
	if (len % 2 == 0)
		return 1;

	Convolution conv;
	vector<float> kernel(len, 1.0 / len);
	conv.SetKernel(kernel, kWidth, kHeight);
	
	// Do convolution
	conv.DoConvolution(sourceImage, destinationImage);

	return 0;
}


int Blur::MedianOpt(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight)
{
	// Height and Width
	int height = sourceImage.rows;
	int width = sourceImage.cols;
	destinationImage = sourceImage.clone();

	//Widthstep
	int widthStep = sourceImage.step[0];
	int nChannels = sourceImage.channels();


	// Offsets
	vector<int> offsets;
	for (int dy = -kHeight / 2; dy <= kHeight / 2; dy++) {
		for (int dx = -kWidth / 2; dx <= kWidth / 2; dx++) {
			offsets.push_back(dy * widthStep + dx);
		}
	}

	// Start -> End
	int xStart = kWidth / 2, xEnd = width - 1 - kWidth / 2;
	int yStart = kHeight / 2, yEnd = height - 1 - kHeight / 2;

	uchar* pSrcData = (uchar*)sourceImage.data;
	uchar* pDesData = (uchar*)destinationImage.data;
	for (int y = yStart; y <= yEnd; y++, pSrcData += widthStep, pDesData += widthStep) {
		uchar* pSrcRow = (uchar*)pSrcData;
		uchar* pDesRow = (uchar*)pDesData;
		for (int x = xStart; x <= xEnd; x++, pSrcRow += nChannels, pDesRow += nChannels)
		{
			uchar* value = new uchar[offsets.size()];
			for (int i = 0; i < offsets.size(); i++)
				value[i] += pSrcRow[offsets[i]];

			sort(value, value + offsets.size() - 1);
			pDesRow[0] = saturate_cast<uchar>(value[offsets.size()/2]);
		}
	}
	return 0;
}


int Blur::GaussianOpt(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight)
{
	vector<float> kernel;
	float sigmaX = 0.3 * ((kWidth - 1) * 0.5 - 1) + 0.8;
	float sigmaY = 0.3 * ((kHeight - 1) * 0.5 - 1) + 0.8;
	float sum = 0;
	for (int dy = -kHeight/2; dy <= kHeight/2; dy++) {
		for (int dx = -kWidth/2; dx <= kWidth/2; dx++) {
			float vX = -(dx * dx) / (2 * sigmaX * sigmaX);
			float vY = -(dy * dy) / (2 * sigmaY * sigmaY);
			kernel.push_back(exp(vX + vY));
			sum = sum + exp(vX + vY);
		}
	}
	for (int i = 0; i < kernel.size(); i++) {
		kernel[i] /= sum;
	}

	Convolution conv;
	conv.SetKernel(kernel, kWidth, kHeight);
	conv.DoConvolution(sourceImage, destinationImage);

	return 0;
}