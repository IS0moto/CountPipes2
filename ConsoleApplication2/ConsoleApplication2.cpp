#include <iostream>
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world454d.lib")
#else
#pragma comment(lib, "opencv_world454.lib")
#endif

int main()
{
	cv::VideoCapture cap(2);//デバイスの番号

	if (!cap.isOpened()) {
		return -1;
	}

	cv::Mat frame;

	while (cap.read(frame)) {
		cv::imshow("win", frame);
		const int key = cv::waitKey(1);
		if (key == 'q') {
			break;
		}
		else if (key == 's') {
			cv::imwrite("img.png", frame);
		}
	}
	cv::destroyAllWindows();
	return 0;
}