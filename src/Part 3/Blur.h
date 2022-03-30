#pragma once
#include "Convolution.h"


class Blur
{
private:
	static int AverageOpt(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight);
	static int MedianOpt(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight);
	static int GaussianOpt(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight);
public:
	static const int AVERAGE_FLAG = 0;
	static const int MEDIAN_FLAG = 1;
	static const int GAUSSIAN_FLAG = 2;

	/*
	Hàm làm trơn ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
	kWidth, kHeight: kích thước kernel
	0: Trung bình
	1: Trung vị
	2: Gaussian
	Hàm trả về
	0: nếu detect thành công
	1: nếu detect thất bại (không đọc được ảnh input,...)
	*/

	static int BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method);
	Blur();
	~Blur();
};
