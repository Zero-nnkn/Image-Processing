#include "GeometricTransformer.h"
#include<iostream>

PixelInterpolate::PixelInterpolate(){}
PixelInterpolate::~PixelInterpolate(){}

BilinearInterpolate::BilinearInterpolate(){}
BilinearInterpolate::~BilinearInterpolate(){}

NearestNeighborInterpolate::NearestNeighborInterpolate(){}
NearestNeighborInterpolate::~NearestNeighborInterpolate(){}

AffineTransform::AffineTransform(){
	this->_matrixTransform = cv::Mat::eye(HEIGHT, WIDTH, CV_32FC1);
}
AffineTransform::~AffineTransform(){}

GeometricTransformer::GeometricTransformer(){}
GeometricTransformer::~GeometricTransformer(){}



/*
Lớp nội suy màu theo phương pháp song tuyến tính
*/
std::vector<uchar> BilinearInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels) 
{
	int l = floor(tx), k = floor(ty);
	int a = tx - l, b = ty - k;
	uchar* pRow0 = pSrc + (k * srcWidthStep + l * nChannels);
	uchar* pRow1 = pSrc + k * srcWidthStep + (l+1) * nChannels;
	uchar* pRow2 = pSrc + (k+1) * srcWidthStep + l * nChannels;
	uchar* pRow3 = pSrc + (k+1) * srcWidthStep + (l+1) * nChannels;

	std::vector<uchar> newColor;

	for (int i = 0; i < nChannels; i++) {
		uchar color = (uchar)((1 - a) * (1 - b) * pRow0[i] + a * (1 - b) * pRow1[i] + b * (1 - a) * pRow2[i] + a * b * pRow3[i]);
		newColor.push_back(color);
	}

	return newColor;
}

/*
Lớp nội suy màu theo phương pháp láng giềng gần nhất
*/
std::vector<uchar> NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels) 
{
	int x = round(tx);
	int y = round(ty);

	uchar* pRow = pSrc + y*srcWidthStep + x*nChannels;
	std::vector<uchar> newColor;

	for (int i = 0; i < nChannels; i++)
		newColor.push_back(pRow[i]);
	
	return newColor;
}


//--------------------------------------------------------------------------------
/*
Lớp biểu diễn pháp biến đổi affine
*/
void AffineTransform::Translate(float dx, float dy)
{
	cv::Mat M = cv::Mat::eye(HEIGHT, WIDTH, CV_32FC1);
	(M.ptr<float>(0) + 2)[0] = dx;
	(M.ptr<float>(1) + 2)[0] = dy;

	this->_matrixTransform = M * this->_matrixTransform;
}

void AffineTransform::Rotate(float angle) 
{
	cv::Mat M = cv::Mat::eye(HEIGHT, WIDTH, CV_32FC1);
	angle = (PI * angle) / 180.0;
	M.ptr<float>(0)[0] = cos(angle);
	(M.ptr<float>(0) + 1)[0] = -sin(angle);
	M.ptr<float>(1)[0] = sin(angle);
	(M.ptr<float>(1) + 1)[0] = cos(angle);

	this->_matrixTransform = M * this->_matrixTransform;
}

void AffineTransform::Scale(float sx, float sy) 
{
	cv::Mat M = cv::Mat::eye(HEIGHT, WIDTH, CV_32FC1);
	M.ptr<float>(0)[0] = sx;
	(M.ptr<float>(1) + 1)[0] = sy;

	this->_matrixTransform = M*this->_matrixTransform;
}

void AffineTransform::TransformPoint(float& x, float& y)
{
	cv::Mat point = cv::Mat::ones(3, 1, CV_32FC1);
	(point.ptr<float>(0))[0] = x;
	(point.ptr<float>(1))[0] = y;

	point = this->_matrixTransform * point;
	x = (point.ptr<float>(0))[0];
	y = (point.ptr<float>(1))[0];
	/*
	for (int y = 0; y < HEIGHT; y++)
	{
		float* pRow = this->_matrixTransform.ptr<float>(y);
		for (int x = 0; x < WIDTH; x++, pRow += 1) {
			cout << pRow[0] << " ";
		}
		cout << endl;
	}
	*/
}



//--------------------------------------------------------------------------------
/*
Lớp thực hiện phép biến đổi hình học trên ảnh
*/
int GeometricTransformer::Transform(
	const Mat& beforeImage,
	Mat& afterImage,
	AffineTransform* transformer,
	PixelInterpolate* interpolator) {

	if (beforeImage.data == NULL)
		return 1;

	int beforeHeight = beforeImage.rows;
	int beforeWidth = beforeImage.cols;
	int height = afterImage.rows;
	int width = afterImage.cols;

	int nChannels = afterImage.channels();
	int beforeWidthStep = (int)beforeImage.step[0];

	uchar* pBeforeData = beforeImage.data;

	for (int y = 0; y < height; y++)
	{
		uchar* pAfterRow = afterImage.ptr<uchar>(y);

		for (int x = 0; x < width; x++, pAfterRow+= nChannels)
		{
			float tx = x, ty = y;
			transformer->TransformPoint(tx, ty);
			if (floor(tx) < beforeWidth - 1 && tx >= 0 && floor(ty) < beforeHeight - 1 && ty >= 0) {
				std::vector<uchar> newColor = interpolator->Interpolate(tx, ty, pBeforeData, beforeWidthStep, nChannels);
				for (int i = 0; i < nChannels; i++) {
					pAfterRow[i] = newColor[i];
				}
			}
		}
	}

	return 0;
}

int GeometricTransformer::RotateKeepImage(
	const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator)
{
	if (srcImage.data == NULL)
		return 1;

	while (angle >= 360)
		angle -= 360;
	while (angle < 0)
		angle += 360;
	
	float temp = angle;
	if (temp >= 180) {
		if (temp >= 270)
			temp = 180 - (temp - 180);
		else
			temp = temp - 180;
	}
	else {
		if (temp >= 90)
			temp = 90 - (temp - 90);
	}
		
	float angleRadian = (PI * temp) / 180.0;

	int newHeight = srcImage.rows * cos(angleRadian) + srcImage.cols * sin(angleRadian);
	int newWidth = srcImage.rows * sin(angleRadian) + srcImage.cols * cos(angleRadian);

	int nChannels = srcImage.channels();
	if (nChannels == 1) {
		dstImage = cv::Mat(newHeight, newWidth, CV_8UC1, Scalar(0));
	}
	else if (nChannels == 3) {
		dstImage = cv::Mat(newHeight, newWidth, CV_8UC3, Scalar(0, 0, 0));
	}
	else if (nChannels == 4) {
		dstImage = cv::Mat(newHeight, newWidth, CV_8UC4, Scalar(0, 0, 0, 0));
	}
	else
		return 1;


	AffineTransform transformer;

	transformer.Translate(-newWidth / 2.0, -newHeight / 2.0);
	transformer.Rotate(-angle);
	transformer.Translate(srcImage.cols / 2.0, srcImage.rows / 2.0);

	if (this->Transform(srcImage, dstImage, &transformer, interpolator) == 1)
		return 1;
	return 0;

}

int GeometricTransformer::RotateUnkeepImage(
	const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator)
{
	if (srcImage.data == NULL)
		return 1;

	while (angle >= 360)
		angle -= 360;
	while (angle < 0)
		angle += 360;

	int nChannels = srcImage.channels();
	if (nChannels == 1) {
		dstImage = cv::Mat(srcImage.rows, srcImage.cols, CV_8UC1, Scalar(0));
	}
	else if (nChannels == 3) {
		dstImage = cv::Mat(srcImage.rows, srcImage.cols, CV_8UC3, Scalar(0, 0, 0));
	}
	else if (nChannels == 4) {
		dstImage = cv::Mat(srcImage.rows, srcImage.cols, CV_8UC4, Scalar(0, 0, 0, 0));
	}
	else
		return 1;

	

	AffineTransform transformer;
	transformer.Translate(-srcImage.cols / 2.0, -srcImage.rows / 2.0);
	transformer.Rotate(-angle);
	transformer.Translate(srcImage.cols / 2.0, srcImage.rows / 2.0);

	if (this->Transform(srcImage, dstImage, &transformer, interpolator) == 1) 
		return 1;
	return 0;

}

int GeometricTransformer::Scale(
	const Mat& srcImage,
	Mat& dstImage,
	float sx, float sy,
	PixelInterpolate* interpolator)
{	
	if (srcImage.data == NULL)
		return 1;
	if (sx <= 0 || sy <= 0)
		return 1;

	int nChannels = srcImage.channels();
	int newHeight = (int)(srcImage.rows * sy);
	int newWidth = (int)(srcImage.cols * sx);
	if (nChannels == 1) {
		dstImage = cv::Mat(newHeight, newWidth, CV_8UC1, Scalar(0));
	}
	else if (nChannels == 3) {
		dstImage = cv::Mat(newHeight, newWidth, CV_8UC3, Scalar(0, 0, 0));
	}
	else if (nChannels == 4) {
		dstImage = cv::Mat(newHeight, newWidth, CV_8UC4, Scalar(0, 0, 0, 0));
	}
	else
		return 1;
	
	AffineTransform transformer;
	transformer.Scale(1.0/sx, 1.0/sy);

	if(this->Transform(srcImage, dstImage, &transformer, interpolator) == 1)
		return 1;
	return 0;
}

int GeometricTransformer::Resize(
	const Mat& srcImage,
	Mat& dstImage,
	int newWidth, int newHeight,
	PixelInterpolate* interpolator) {

	if (srcImage.data == NULL)
		return 1;
	if (newWidth <= 0 || newHeight <= 0)
		return 1;

	int nChannels = srcImage.channels();
	if (nChannels == 1) {
		dstImage = cv::Mat(newHeight, newWidth, CV_8UC1, Scalar(0));
	}
	else if (nChannels == 3) {
		dstImage = cv::Mat(newHeight, newWidth, CV_8UC3, Scalar(0, 0, 0));
	}
	else if (nChannels == 4) {
		dstImage = cv::Mat(newHeight, newWidth, CV_8UC4, Scalar(0, 0, 0, 0));
	}
	else
		return 1;

	AffineTransform transformer;
	transformer.Scale(srcImage.cols * 1.0 / newWidth, srcImage.rows*1.0/newHeight);
	
	if(this->Transform(srcImage, dstImage, &transformer, interpolator) == 1)
		return 1;
	return 0;
}

int GeometricTransformer::Flip(
	const Mat& srcImage,
	Mat& dstImage,
	bool direction,
	PixelInterpolate* interpolator)
{
	if (srcImage.data == NULL)
		return 1;

	int nChannels = srcImage.channels();
	if (nChannels == 1) {
		dstImage = cv::Mat(srcImage.rows, srcImage.cols, CV_8UC1, Scalar(0));
	}
	else if (nChannels == 3) {
		dstImage = cv::Mat(srcImage.rows, srcImage.cols, CV_8UC3, Scalar(0, 0, 0));
	}
	else if (nChannels == 4) {
		dstImage = cv::Mat(srcImage.rows, srcImage.cols, CV_8UC4, Scalar(0, 0, 0, 0));
	}
	else
		return 1;

	AffineTransform transformer;
	if (direction == 1) // Trục ngang
	{
		transformer.Scale(1, -1);
		transformer.Translate(0, srcImage.rows);
	}
	else //Trục đứng
	{
		transformer.Scale(-1, 1);
		transformer.Translate(srcImage.cols, 0);
	}

	if(this->Transform(srcImage, dstImage, &transformer, interpolator) == 1)
		return 1;
	return 0;
}