#pragma once
#include "Convolution.h"

class Blur
{
private:
	int AverageOpt(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight);
	int MedianOpt(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight);
	int GaussianOpt(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight);
public:
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
	int BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method);
	Blur();
	~Blur();
};
