﻿#pragma once
#include "Convolution.h"

class EdgeDetector
{
private:
	int SobelOpt(const Mat& sourceImage, Mat& destinationImage);
	int PrewittOpt(const Mat& sourceImage, Mat& destinationImage);
	int LaplaceOpt(const Mat& sourceImage, Mat& destinationImage);
public:
	/*
	Hàm phát hiện biên cạnh của ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
		1: Sobel
		2: Prewitt
		3: Laplace
	Hàm trả về
		0: nếu detect thành công
		1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int DetectEdge(const Mat& sourceImage, Mat& destinationImage, int method);
	EdgeDetector();
	~EdgeDetector();
};

