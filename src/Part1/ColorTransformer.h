#pragma once

#include<opencv2/opencv.hpp>
#include <map>
using namespace cv;

#define NUM_COLOR 256
#define DELTA 128
#define HISTOGRAM_HEIGHT 640
#define HISTOGRAM_WIDTH 256*3

class ColorTransformer
{
public:
	/*
	Hàm nhận vào 1 số, trả về giá trị số đó nếu nằm trong [0, 255]
		Nếu vượt quá trả về 255
		Nếu nhỏ hơn trả về 0
	*/
	int SqueezeValue(int v);

	/*
	Hàm nhận vào một ma trận có channel = 1 và trả về giá trị lớn nhất của ma trận
	*/
	int getMax(const Mat& histMatrix);

	/*
	Hàm nhận vào một ảnh, thay đổi độ sáng của ảnh này và lưu kết quả vào ảnh mới
	Tham so:
		sourceImage	: ảnh ban đầu
		destinationImage: ảnh kết quả
		b	: giá trị số nguyên dùng để thay đổi độ sáng của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	void ChangeBrighnessChannel(const Mat& sourceImage, Mat& destinationImage, short b, int channel);
	int ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b);

	/*
	Hàm nhận vào một ảnh, thay đổi độ tương phản của ảnh này và lưu kết quả vào ảnh mới
	Tham so :
		sourceImage : ảnh ban đầu
		destinationImage : ảnh kết quả
		c	: giá trị số thực dùng để thay đổi độ tương phản của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int ChangeContrast(const Mat& sourceImage, Mat& destinationImage, double c);
	void ChangeContrastChannel(const Mat& sourceImage, Mat& destinationImage, double c, int channel);


	/*
	Hàm tính lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
		sourceImage : ảnh ban đầu
		histMatrix : ma trận histogram (nChannels x 256), mỗi dòng là 1 mảng 256 phần tử histogram của từng kênh màu
	Hàm trả về:
		1: Nếu thành công thì trả về matrix kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tính được histogram hoặc ảnh input không tồn tại
	*/
	int CalcHistogram(const Mat& sourceImage, Mat& histMatrix);
	void CalcHistogramChannel(const Mat& sourceImage, Mat& histMatrix, int channel);

	/*
	Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
		image : ảnh dùng để tính histogram
		histImage : ảnh histogram
	Hàm trả về:
		1: Nếu thành công vẽ được histogram
		0: Nếu không vẽ được histogram
	*/
	int HistogramEqualization(const Mat& sourceImage, Mat& destinationImage);

	/*
	Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
		histMatrix : ma trận histogram đã tính được
		histImage : ảnh histogram được vẽ
	Hàm trả về:
		1: Nếu thành công vẽ được histogram
		0: Nếu không vẽ được histogram
	*/
	int DrawHistogram(const Mat& histMatrix, Mat& histImage);

	// Scale histogram matrix to [0, HISTOGRAM_HEIGHT - DELTA]
	void scaleHistMatrix(Mat& histMatrix);

	ColorTransformer();
	~ColorTransformer();
};


