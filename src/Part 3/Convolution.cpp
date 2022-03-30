#include "Convolution.h"

Convolution::Convolution() {
	this->_kernel = vector<float>(0, 0);
	this->_kernelHeight = 0;
	this->_kernelWidth = 0;
}

Convolution::~Convolution(){}


// Set and Get Kernel
vector<float> Convolution::GetKernel() {
	return this->_kernel;
}

void Convolution::SetKernel(vector<float> kernel, int kWidth, int kHeight) {
	if (kWidth * kHeight % 2 == 0)
		return;
	this->_kernel = kernel;
	this->_kernelHeight = kHeight;
	this->_kernelWidth = kWidth;
}

int Convolution::DoConvolution(const Mat& sourceImage, Mat& destinationImage, bool isAbsolute) {
	if (sourceImage.empty() || sourceImage.channels() != 1)
		return 1;

	// Height and Width
	int height = sourceImage.rows;
	int width = sourceImage.cols;
	destinationImage = sourceImage.clone();

	//Widthstep
	int widthStep = sourceImage.step[0];
	int nChannels = sourceImage.channels();

	// Offsets
	vector<int> offsets;
	for (int dy = -this->_kernelHeight / 2; dy <= this->_kernelHeight / 2; dy++) {
		for (int dx = -this->_kernelWidth / 2; dx <= this->_kernelWidth / 2; dx++) {
			offsets.push_back(dy * widthStep + dx);
		}
	}

	// Start -> End
	int xStart = this->_kernelWidth / 2, xEnd = width - 1 - this->_kernelWidth / 2;
	int yStart = this->_kernelHeight / 2, yEnd = height - 1 - this->_kernelHeight / 2;

	uchar* pSrcData = (uchar*)sourceImage.data + yStart * widthStep;
	uchar* pDesData = (uchar*)destinationImage.data + yStart * widthStep;
	for (int y = yStart; y <= yEnd; y++, pSrcData += widthStep, pDesData += widthStep) {
		uchar* pSrcRow = (uchar*)pSrcData + xStart * nChannels;
		uchar* pDesRow = (uchar*)pDesData + xStart * nChannels;
		for (int x = xStart; x <= xEnd; x++, pSrcRow += nChannels, pDesRow += nChannels)
		{
			float value = 0;
			for (int i = 0; i < offsets.size(); i++)
				value += pSrcRow[offsets[i]] * this->_kernel[offsets.size() - 1 - i];

			pDesRow[0] = isAbsolute ? saturate_cast<uchar>(abs(value)) : saturate_cast<uchar>(value);
		}
	}
}

