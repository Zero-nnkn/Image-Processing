#include "Canny.h"
#include "Blur.h"
#include "EdgeDetector.h"

#define PI 3.14159265

Canny::Canny() {}
Canny::~Canny() {}


int Canny::findIntensityGradient(const Mat& sourceImage, Mat& edgeGradient, Mat& angle, int method) 
{
	
	if (sourceImage.empty() || sourceImage.type() != CV_8UC1)
		return 1;

	switch (method)
	{
	case Canny::SOBEL_FLAG:
	{
		if (Canny::SobelIntensityGradient(sourceImage, edgeGradient, angle) != 0)
			return 1;
		break;
	}
	case Canny::PREWITT_FLAG:
	{
		if (Canny::PrewittIntensityGradient(sourceImage, edgeGradient, angle) != 0)
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

int Canny::SobelIntensityGradient(const Mat& sourceImage, Mat& edgeGradient, Mat& angle)
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
	conX.DoConvolution(sourceImage, Gx, true);
	conY.DoConvolution(sourceImage, Gy, true);

	// Height, Width
	int height = sourceImage.rows;
	int width = sourceImage.cols;
	edgeGradient = Mat(height, width, CV_8UC1);
	angle = Mat(height, width, CV_32FC1);

	int count = 0;
	int c = 0;
	for (int y = 0; y < height; y++)
	{
		uchar* pEdge = edgeGradient.ptr<uchar>(y);
		float* pAngle = angle.ptr<float>(y);
		uchar* pX = Gx.ptr<uchar>(y);
		uchar* pY = Gy.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			pEdge[x] = saturate_cast<uchar>(sqrt(pX[x] * pX[x] + pY[x] * pY[x]));
			// Angle [-90, 90]
			pAngle[x] = saturate_cast<float>(atan2(pY[x], pX[x]) * 180 / PI - 90);
		}
	}

	return 0;
}

int Canny::PrewittIntensityGradient(const Mat & sourceImage, Mat & edgeGradient, Mat & angle)
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
	conX.DoConvolution(sourceImage, Gx, true);
	conY.DoConvolution(sourceImage, Gy, true);

	// Height, Width
	int height = sourceImage.rows;
	int width = sourceImage.cols;
	edgeGradient = Mat(height, width, CV_8UC1);
	angle = Mat(height, width, CV_32FC1);

	for (int y = 0; y < height; y++)
	{
		uchar* pEdge = edgeGradient.ptr<uchar>(y);
		float* pAngle = angle.ptr<float>(y);
		uchar* pX = Gx.ptr<uchar>(y);
		uchar* pY = Gy.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			pEdge[x] = saturate_cast<uchar>(sqrt(pX[x] * pX[x] + pY[x] * pY[x]));
			// Angle [-90, 90]
			pAngle[x] = saturate_cast<float>(atan2(pY[x], pX[x]) * 180 / PI - 90);
		}
	}

	return 0;
}


int Canny::ApplyNMS(const Mat& magnitude, const Mat& angle, Mat& nms) 
{
	if (magnitude.empty() || magnitude.type() != CV_8UC1)
		return 1;

	if (angle.empty() || angle.type() != CV_32FC1)
		return 1;

	Mat direction;
	if (Canny::AngleToDirection(angle, direction) == 1)
		return 1;

	int height = magnitude.rows;
	int width = magnitude.cols;
	nms = Mat(height, width, CV_8UC1, Scalar(255));

	Mat paddedMagnitude;
	copyMakeBorder(magnitude, paddedMagnitude, 1, 1, 1, 1, BORDER_CONSTANT);

	for (int y = 0; y < height; y++)
	{
		const uchar* pMagnitude = paddedMagnitude.ptr<uchar>(y + 1);
		const uchar* pMagnitudePrev = paddedMagnitude.ptr<uchar>(y);
		const uchar* pMagnitudeNext = paddedMagnitude.ptr<uchar>(y + 2);
		uchar* pDirection = direction.ptr<uchar>(y);
		uchar* pNms = nms.ptr<uchar>(y);
		
		for (int x = 0; x < width; x++, pDirection++, pNms++) {
			if (IsLocalMax(pDirection[0], pMagnitude, pMagnitudePrev, pMagnitudeNext, x + 1) == false)
				pNms[0] = 0;
		}
	}

	bitwise_and(magnitude, nms, nms);
	return 0;
}


int Canny::AngleToDirection(const Mat& angle, Mat& direction) {
	if (angle.empty() || angle.type() != CV_32FC1)
		return 1;

	int height = angle.rows;
	int width = angle.cols;
	direction = Mat(height, width, CV_8UC1);

	for (int y = 0; y < height; y++)
	{
		const float* pAngle = angle.ptr<float>(y);
		uchar* pDirection = direction.ptr<uchar>(y);
		
		for (int x = 0; x < width; x++, pAngle++, pDirection++) {
			pDirection[0] = AngleToDirectionPixel(pAngle[0]);
		}
	}

	return 0;
}


int Canny::AngleToDirectionPixel(float angle) {
	if (angle <= -22.5f && angle > -67.5f)
		return UP_DIAGONAL_DIRECTION;

	if (angle <= 22.5f && angle > -22.5f)
		return HORIZONTAL_DIRECTION;

	if (angle <= 67.5f && angle > 22.5f)
		return DOWN_DIAGONAL_DIRECTION;

	return VERTICAL_DIRECTION;
}


bool Canny::IsLocalMax(int direction, const uchar* row, const uchar* rowPrev, const uchar* rowNext, int offset) {
	uchar value = row[offset];
	uchar near1 = 0;
	uchar near2 = 0;

	if (direction == HORIZONTAL_DIRECTION) {
		near1 = row[offset - 1];
		near2 = row[offset + 1];
	}

	else if (direction == VERTICAL_DIRECTION) {
		near1 = rowPrev[offset];
		near2 = rowNext[offset];
	}

	else if (direction == UP_DIAGONAL_DIRECTION) {
		near1 = rowPrev[offset + 1];
		near2 = rowNext[offset - 1];
	}

	else if (direction == DOWN_DIAGONAL_DIRECTION) {
		near1 = rowPrev[offset - 1];
		near2 = rowNext[offset + 1];
	}

	return value >= near1 && value >= near2;
}


int Canny::ApplyThreshold(const Mat& nms, Mat& destinationImage, float lowThreshold, float highThreshold)
{
	if (nms.empty() || nms.type() != CV_8UC1)
		return 1;

	if (lowThreshold > 255)
		lowThreshold = 255;
	if (highThreshold > 255)
		highThreshold = 255;

	int height = nms.rows;
	int width = nms.cols;
	
	Mat strongImage = Mat(height, width, CV_8UC1, Scalar(0));;
	Mat weakImage = Mat(height, width, CV_8UC1, Scalar(0));;

	// Double Threshold
	for (int y = 0; y < height; y++)
	{
		const uchar* pNms = nms.ptr<uchar>(y);
		uchar* pStrong = strongImage.ptr<uchar>(y);
		uchar* pWeak = weakImage.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			if (pNms[x] >= saturate_cast<uchar>(highThreshold))
				pStrong[x] = 255;
			else if (pNms[x] < saturate_cast<uchar>(lowThreshold))
				continue;
			else
				pWeak[x] = 255;
		}
	}


	// Hysteresis
	Mat paddedStrongImage;
	copyMakeBorder(strongImage, paddedStrongImage, 1, 1, 1, 1, BORDER_CONSTANT);

	int changeCount = 1;
	while (changeCount) 
	{
		changeCount = 0;
		for (int y = 0; y < height; y++)
		{
			uchar* pStrong = paddedStrongImage.ptr<uchar>(y + 1);
			uchar* pStrongPrev = paddedStrongImage.ptr<uchar>(y);
			uchar* pStrongNext = paddedStrongImage.ptr<uchar>(y + 2);
			uchar* pStrongOrigin = strongImage.ptr<uchar>(y);
			uchar* pWeak = weakImage.ptr<uchar>(y);

			for (int x = 0; x < width; x++) {
				if (pWeak[x] == 255 && pStrongOrigin[x] == 0 && Canny::IsNearStrong(x + 1, pStrong, pStrongPrev, pStrongNext)) {
					pStrongOrigin[x] = 255;
					changeCount += 1;
					pWeak[x] = 0;
				}
			}
		}
	}

	destinationImage = strongImage;
	return 0;
}

bool Canny::IsNearStrong(int index, uchar* pStrong, uchar* pStrongPrev, uchar* pStrongNext)
{
	int check_sum = pStrong[index - 1] + pStrong[index + 1]
		+ pStrongPrev[index - 1] + pStrong[index] + pStrongPrev[index + 1]
		+ pStrongNext[index - 1] + pStrongNext[index] + pStrongNext[index + 1];
	if (check_sum > 0) {
		return true;
	}
	return false;
}

#include "EdgeDetector.h"
int Canny::DetectEdge(const Mat& sourceImage, Mat& destinationImage, float lowThreshold, float highThreshold) {
	if (sourceImage.empty() || sourceImage.type() != CV_8UC1)
		return 1;
	
	// Gaussian Blur
	Mat blurred;
	if (Blur::BlurImage(sourceImage, blurred, 5, 5, Blur::GAUSSIAN_FLAG) == 1)
		return 1;
	imshow("blur", blurred);
	waitKey(0);


	// Sobel edge detector
	Mat magnitude, angle;
	if (Canny::findIntensityGradient(blurred, magnitude, angle, Canny::SOBEL_FLAG) == 1)
		return 1;
	imshow("mag", magnitude);
	waitKey(0);

	// Non-max suppression
	Mat nms;
	if (Canny::ApplyNMS(magnitude, angle, nms) == 1)
		return 1;
	imshow("non max", nms);
	waitKey(0);

	// Hysteresis Thresholding
	if (Canny::ApplyThreshold(nms, destinationImage, lowThreshold, highThreshold) == 1)
		return 1;

	return 0;
} 