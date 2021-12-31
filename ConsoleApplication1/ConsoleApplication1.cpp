#include <iostream>
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world454d.lib")
#else
#pragma comment(lib, "opencv_world454.lib")
#endif

int main()
{
	const char* windowName = "Image";
	cv::namedWindow(windowName,cv::WINDOW_NORMAL);
	cv::Mat img = cv::imread("C:\\Users\\isoff\\WORK_SPACE\\image.png");
	cv::Mat src;
	cv::cvtColor(img, src, cv::COLOR_RGB2GRAY);
	cv::setWindowProperty(windowName,cv::WND_PROP_FULLSCREEN,cv::WINDOW_FULLSCREEN);
	cv::imshow(windowName, src);

	cv::waitKey(0);

	return 0;
}