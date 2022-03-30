#pragma once
#include "Convolution.h"

class EdgeDetector
{
private:
	static int SobelOpt(const Mat& sourceImage, Mat& destinationImage);
	static int PrewittOpt(const Mat& sourceImage, Mat& destinationImage);
	static int LaplaceOpt(const Mat& sourceImage, Mat& destinationImage);

public:
	static const int SOBEL_FLAG = 0;
	static const int PREWITT_FLAG = 1;
	static const int LAPLACE_FLAG = 2;

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
	static int DetectEdge(const Mat& sourceImage, Mat& destinationImage, int method);
	EdgeDetector();
	~EdgeDetector();
};

