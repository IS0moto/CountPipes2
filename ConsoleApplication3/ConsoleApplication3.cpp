#include <iostream>
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world454d.lib")
#else
#pragma comment(lib, "opencv_world454.lib")
#endif

void ChangeHough(int state, void* userdata);
void ChangeHoughParam1(int pos, void* userdata);


class CountParam {
	public:
		int thresh = 0;
		int maxval = 0;
		int houghDp=0;
		int houghMinDist=0;
		int houghParam1=0;
		int houghParam2=0;
		int houghMinRadius=0;
		int houghMaxRadius=0;
		int updateKey = 0;

};

CountParam param1;

cv::Mat src_gray;
cv::Mat src_bin;
cv::Mat src_canny;
cv::Mat src;

int pam1 = 0;
int pam2 = 1;

int main(){

	src = cv::imread("C:\\Users\\isoff\\WORK_SPACE\\ConsoleApplicationSample\\OSAKAPICTURE\\img3.jpg");
	if (src.empty() == true) {
		return -1;
	}
	cv::Mat src_clone = src.clone();
	param1.thresh = 150;
	param1.maxval = 200;
	param1.houghDp = 4;
	param1.houghMinDist = 50;
	param1.houghParam1 = 50;
	param1.houghParam2 = 100;
	param1.updateKey = 0;

	
	cv::namedWindow("Hough", cv::WINDOW_NORMAL);
	cv::namedWindow("bin", cv::WINDOW_NORMAL);
		cv::setWindowProperty("Hough", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FREERATIO);
	cv::setWindowProperty("bin", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FREERATIO);

	cv::namedWindow("Trackbar");
	cv::createTrackbar("thresh", "Trackbar", &param1.thresh, 300, ChangeHoughParam1);
	cv::createTrackbar("maxval", "Trackbar", &param1.maxval, 300, ChangeHoughParam1);
	cv::createTrackbar("Dp","Trackbar",&param1.houghDp,5, ChangeHoughParam1);
	cv::createTrackbar("MinDist", "Trackbar", &param1.houghMinDist, 100, ChangeHoughParam1);
	cv::createTrackbar("param1", "Trackbar", &param1.houghParam1, 100, ChangeHoughParam1);
	cv::createTrackbar("param2", "Trackbar", &param1.houghParam2, 100, ChangeHoughParam1);
	cv::createTrackbar("update", "Trackbar", &param1.updateKey, 1, ChangeHough);

	cv::GaussianBlur(src, src, cv::Size(5, 5), 0);
	cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
	threshold(src_gray, src_bin, param1.thresh, param1.maxval, cv::THRESH_BINARY);

	cv::imshow("bin", src_bin);
	cv::resizeWindow("bin", 640, 480);

	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(src_bin, circles, cv::HOUGH_GRADIENT, param1.houghDp, param1.houghMinDist, param1.houghParam1, param1.houghParam2, 10, 30);
	int count = circles.size();
	printf("Count::%d\n", count);

	for (int i = 0; i < circles.size(); i++) {
		cv::circle(src_clone, cv::Point(circles[i][0], circles[i][1]), circles[i][2], cv::Scalar(0, 255, 255), 5, cv::LINE_AA);
	}
	cv::imwrite("img.png", src_clone);
	cv::imshow("Hough", src_clone);

	cv::resizeWindow("Hough", 640, 480);

	while (1) {
		
		if (pam1!=pam2) {
			
			cv::destroyWindow("bin");
			cv::destroyWindow("Hough");

			src_clone = src.clone();
			cv::namedWindow("bin", cv::WINDOW_NORMAL);
			cv::setWindowProperty("bin", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FREERATIO);
			cv::namedWindow("Hough", cv::WINDOW_NORMAL);
			cv::setWindowProperty("Hough", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FREERATIO);

			threshold(src_gray, src_bin, param1.thresh, param1.maxval, cv::THRESH_BINARY);
			std::vector<cv::Vec3f> circles;
			cv::HoughCircles(src_bin, circles, cv::HOUGH_GRADIENT, param1.houghDp, param1.houghMinDist, param1.houghParam1, param1.houghParam2, 10, 30);
			printf("houghParam1 %d \n",param1.houghParam1);
			int count = circles.size();
			printf("Count::%d\n", count);

			for (int i = 0; i < circles.size(); i++) {
				cv::circle(src_clone, cv::Point(circles[i][0], circles[i][1]), circles[i][2], cv::Scalar(0, 255, 255), 5, cv::LINE_AA);
			}
			cv::imwrite("img.png", src_clone);
			cv::imshow("bin",src_bin);
			cv::imshow("Hough", src_clone);

			cv::resizeWindow("Hough", 640, 480);

			pam2 = pam1;
		}
		cv::waitKey(5);
	}
	
	return 0;
}

void ChangeHoughParam1(int pos , void* userdata) {
	printf("param1.houghParam1 %d\n",param1.houghParam1);
}

void ChangeHough(int state, void* userdata) {
	//printf("param1.updateKey %d\n", param1.updateKey);
	//param1.updateKey = 0;
	//cv::setTrackbarPos("update","Trackbar",0);
	if (pam1 == 0)
		pam1 = 1;
	else
		pam1 = 0;

	
}