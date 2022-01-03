#include <iostream>
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world454d.lib")
#else
#pragma comment(lib, "opencv_world454.lib")
#endif

int main()
{
	cv::VideoCapture cap(0);//デバイスの番号


	if (!cap.isOpened()) {
		return -1;
	}

	int radius = 50;

	cv::namedWindow("Trackbar");
	cv::createTrackbar("Radius", "Trackbar", &radius, 50);
	cv::Mat frame;
	cv::Mat img;
	

	while (cap.read(frame)) {
		int width = frame.cols;
		int height = frame.rows;
		img = frame.clone();
		printf("width %d height %d",width,height);
		cv::circle(img, cv::Point(width/2, height / 2), radius, cv::Scalar(0, 255, 255), 5, cv::LINE_AA);
		cv::imshow("win", img);
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