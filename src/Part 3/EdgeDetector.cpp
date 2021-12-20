#include "EdgeDetector.h"

#define PERCENT_THRESHOLD 0.075;
#define PERCENT_THRESHOLD_LAPLACE 0.15;

EdgeDetector::EdgeDetector(){}
EdgeDetector::~EdgeDetector(){}

int EdgeDetector::DetectEdge(const Mat& sourceImage, Mat& destinationImage, int method) {
	if (sourceImage.empty() || sourceImage.channels() != 1)
		return 1;

	switch (method)
	{
	case 0:
	{
		if (this->SobelOpt(sourceImage, destinationImage) == 1)
			return 1;
		break;
	}
	case 1:
	{
		if (this->PrewittOpt(sourceImage, destinationImage) == 1)
			return 1;
		break;
	}
	case 2:
	{
		if (this->LaplaceOpt(sourceImage, destinationImage) == 1)
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

int EdgeDetector::SobelOpt(const Mat& sourceImage, Mat& destinationImage) 
{
	Convolution conX, conY;
	Mat Gx, Gy;
	vector<float> Wx = { 1, 0, -1, 2, 0, -2, 1, 0, -1 };
	vector<float> Wy = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
	for (int i = 0; i < Wx.size(); i++)
	{
		Wx[i] /= 4.0;
		Wy[i] /= 4.0;
	}

	// Convolution
	conX.SetKernel(Wx, 3, 3);
	conY.SetKernel(Wy, 3, 3);
	conX.DoConvolution(sourceImage, Gx);
	conY.DoConvolution(sourceImage, Gy);

	// Height, Width
	int height = sourceImage.rows;
	int width = sourceImage.cols;
	Mat grad = Gx.clone();
	float threshold = 0;

	for (int y = 0; y < height; y++)
	{
		uchar* pGrad = grad.ptr<uchar>(y);
		uchar* pX = Gx.ptr<uchar>(y);
		uchar* pY = Gy.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			pGrad[x] = saturate_cast<uchar>(sqrt(pX[x] * pX[x] + pY[x] * pY[x]));
			if (pGrad[x] > saturate_cast<uchar>(threshold))
				threshold = (float)pGrad[x];
		}
	}

	// Threshold = p*MaxGrad
	threshold *= PERCENT_THRESHOLD;

	// BEI
	destinationImage = sourceImage.clone();
	for (int y = 0; y < height; y++)
	{
		uchar* pDes = destinationImage.ptr<uchar>(y);
		uchar* pGrad = grad.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			if (pGrad[x] >= saturate_cast<uchar>(threshold))
				pDes[x] = 255;
			else
				pDes[x] = 0;
		}
	}

	return 0;
}

int EdgeDetector::PrewittOpt(const Mat& sourceImage, Mat& destinationImage)
{
	Convolution conX, conY;
	Mat Gx, Gy;
	vector<float> Wx = { 1, 0, -1, 1, 0, -1, 1, 0, -1 };
	vector<float> Wy = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	for (int i = 0; i < Wx.size(); i++)
	{
		Wx[i] /= 3.0;
		Wy[i] /= 3.0;
	}

	// Convolution
	conX.SetKernel(Wx, 3, 3);
	conY.SetKernel(Wy, 3, 3);
	conX.DoConvolution(sourceImage, Gx);
	conY.DoConvolution(sourceImage, Gy);

	// Height, Width
	int height = sourceImage.rows;
	int width = sourceImage.cols;
	Mat grad = Gx.clone();
	float threshold = 0;

	for (int y = 0; y < height; y++)
	{
		uchar* pGrad = grad.ptr<uchar>(y);
		uchar* pX = Gx.ptr<uchar>(y);
		uchar* pY = Gy.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			pGrad[x] = saturate_cast<uchar>(sqrt(pX[x] * pX[x] + pY[x] * pY[x]));
			if (pGrad[x] > saturate_cast<uchar>(threshold))
				threshold = (float)pGrad[x];
		}
	}

	// Threshold = p*MaxGrad
	threshold *= PERCENT_THRESHOLD;

	// BEI
	destinationImage = sourceImage.clone();
	for (int y = 0; y < height; y++)
	{
		uchar* pDes = destinationImage.ptr<uchar>(y);
		uchar* pGrad = grad.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			if (pGrad[x] >= saturate_cast<uchar>(threshold))
				pDes[x] = 255;
			else
				pDes[x] = 0;
		}
	}

	return 0;
}

int EdgeDetector::LaplaceOpt(const Mat& sourceImage, Mat& destinationImage)
{
	Convolution conv;
	Mat grad;
	vector<float> W = { 0, 1, 0, 1, -4, 1, 0, 1, 0	};

	// Convolution
	conv.SetKernel(W, 3, 3);

	conv.DoConvolution(sourceImage, grad);

	// Height, Width
	int height = sourceImage.rows;
	int width = sourceImage.cols;
	int widthStep = grad.step[0];
	int nChannels = grad.channels();

	float threshold = 0;
	for (int y = 0; y < height; y++)
	{
		uchar* pGrad = grad.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			if (pGrad[x] > saturate_cast<uchar>(threshold))
				threshold = (float)pGrad[x];
		}
	}

	// Threshold = 30%*MaxGrad
	threshold *= PERCENT_THRESHOLD_LAPLACE;

	// BEI
	destinationImage = sourceImage.clone();
	for (int y = 0; y < height; y++)
	{
		uchar* pDes = destinationImage.ptr<uchar>(y);
		uchar* pGrad = grad.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			if (pGrad[x] >= saturate_cast<uchar>(threshold))
				pDes[x] = 255;
			else
				pDes[x] = 0;
		}
	}

	return 0;
}