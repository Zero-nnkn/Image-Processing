#pragma once
#include "Convolution.h"

class Canny {
private:
	static int SobelIntensityGradient(const Mat& sourceImage, Mat& edgeGradient, Mat& angle);
	static int PrewittIntensityGradient(const Mat& sourceImage, Mat& edgeGradient, Mat& angle);

public:
	static const int SOBEL_FLAG = 0;
	static const int PREWITT_FLAG = 1;
	static const int VERTICAL_DIRECTION = 0;
	static const int HORIZONTAL_DIRECTION = 1;
	static const int UP_DIAGONAL_DIRECTION = 2;
	static const int DOWN_DIAGONAL_DIRECTION = 3;
	
	/*
	Hàm trả về Hướng của gradient bằng toán tử Sobel:
	sourceImage : ảnh input
	edgeGradient : kết quả cường độ của gradient = sqrt( Gx^2 + Gy^2)
	angle: theta = kết quả hướng của gradient = atan^-1(Gy / Gx)
	method: phương pháp tìm
		1: Sobel
		2: Prewitt
	Hàm trả về
		0: nếu tìm thành công
		1: nếu tìm thất bại (không đọc được ảnh input,...)
	*/
	static int findIntensityGradient(const Mat& sourceImage, Mat& edgeGradient, Mat& angle, int method);

	static int ApplyNMS(const Mat& amplitude, const Mat& direction, Mat& nms);
	static int AngleToDirection(const Mat& angle, Mat& direction);
	static int AngleToDirectionPixel(float angle);
	static bool IsLocalMax(int direction, const uchar* row, const uchar* rowPrev, const uchar* rowNext, int offset);
	static bool IsNearStrong(int index, uchar* pStrong, uchar* pStrongPrev, uchar* pStrongNext);
	static int ApplyThreshold(const Mat& nms, Mat& destinationImage, float lowThreshold, float highThreshold);


	/*
	Hàm phát hiện biên cạnh của ảnh xám bằng phương pháp Canny
	sourceImage: ảnh input
	destinationImage: ảnh output
	Hàm trả về
		0: nếu detect thành công
		1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	static int DetectEdge(const Mat& sourceImage, Mat& destinationImage, float lowThreshold, float highThreshold);



	Canny();
	~Canny();
};