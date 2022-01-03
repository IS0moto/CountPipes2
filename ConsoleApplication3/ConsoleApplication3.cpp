#include <iostream>
#include <opencv2/opencv.hpp>
#include<time.h>
#include<string.h>


#ifdef _DEBUG
#pragma comment(lib, "opencv_world454d.lib")
#else
#pragma comment(lib, "opencv_world454.lib")
#endif

using namespace cv;

cv::Mat PrepareImg(cv::Mat dst);
cv::Mat AddCount(cv::Mat dst,int count);
void ChangeHough(int state, void* userdata);
void CallbackButton(int state, void* userdata);
void ChangeHoughParam1(int pos, void* userdata);
void ChangeImg(int pos, void* userdata);
void CircleDistance(std::vector<cv::Vec3f>,int* ,int ,int);

#define RATE_R 5
#define RATE_D 1.1


class CountParam {
public:
	int houghDp = 0;
	int houghMinDist = 0;
	int houghParam1 = 0;
	int houghParam2 = 0;
	int houghRadius = 0;
	int switchString = 0;
	int updateKey = 0;
	int houghRate = 0;

};

CountParam param1;

cv::Mat src_gray;
cv::Mat src_bin;
cv::Mat src_canny;

int pam1 = 0;
int pam2 = 1;

int main()
{
	cv::Mat src;

	param1.houghDp = 1;
	param1.houghParam1 = 50;
	param1.houghParam2 = 15;
	param1.houghRadius = 20;
	param1.switchString = 0;
	param1.updateKey = 0;
	param1.houghRate = 1;

	src = PrepareImg(src);

	param1.houghMinDist = 2 * param1.houghRadius;
	
	int width = src.cols;
	int height = src.rows;
	cv::Mat src_clone = src.clone();


	cv::namedWindow("Hough", cv::WINDOW_NORMAL);
	cv::namedWindow("bin", cv::WINDOW_NORMAL);
	cv::setWindowProperty("Hough", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FREERATIO);
	cv::setWindowProperty("bin", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FREERATIO);

	cv::namedWindow("Trackbar");
	cv::createTrackbar("Dp", "Trackbar", &param1.houghDp, 10, ChangeHoughParam1);
	cv::createTrackbar("MinDist", "Trackbar", &param1.houghMinDist, 100, ChangeHoughParam1);
	cv::createTrackbar("param1", "Trackbar", &param1.houghParam1, 100, ChangeHoughParam1);
	cv::createTrackbar("param2", "Trackbar", &param1.houghParam2, 100, ChangeHoughParam1);
	cv::createTrackbar("houghRadius", "Trackbar", &param1.houghRadius, 100, ChangeHoughParam1);
	cv::createTrackbar("switchString", "Trackbar", &param1.switchString, 1, ChangeHough);
	cv::createTrackbar("houghRate", "Trackbar", &param1.houghRate, 4, ChangeHough);

	char nameb1[] = "button1";
	cv::createButton(nameb1, CallbackButton);

	cv::createTrackbar("update", "Trackbar", &param1.updateKey, 1, ChangeHough);

	//cv::resizeWindow("Trackbar", 400, 600);


	cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(src_gray, src_bin, cv::Size(5, 5), 0);
	
	cv::Canny(src_bin, src_canny, param1.houghParam1 * param1.houghRate, param1.houghParam2 * param1.houghRate);

	cv::imshow("bin", src_canny);
	cv::resizeWindow("bin", 640, 480);

	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(src_bin, circles, cv::HOUGH_GRADIENT,
		param1.houghDp,
		param1.houghMinDist * param1.houghRate,
		param1.houghParam1 * param1.houghRate,
		param1.houghParam2 * param1.houghRate,
		param1.houghRadius - RATE_R, param1.houghRadius + RATE_R);

	int count = circles.size();
	printf("Count::%d\n", count);

	for (int i = 0; i < circles.size(); i++) {
		cv::circle(src_clone, cv::Point(circles[i][0], circles[i][1]), circles[i][2], cv::Scalar(0, 255, 255), 5, cv::LINE_AA);
	}
	cv::imwrite("img.png", src_clone);
	cv::imshow("Hough", src_clone);

	cv::resizeWindow("Hough", 640, 480);

	while (1) {

		if (pam1 != pam2) {

			cv::destroyWindow("bin");
			cv::destroyWindow("Hough");

			src_clone = src.clone();
			cv::namedWindow("bin", cv::WINDOW_NORMAL);
			cv::setWindowProperty("bin", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FREERATIO);
			cv::namedWindow("Hough", cv::WINDOW_NORMAL);
			cv::setWindowProperty("Hough", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FREERATIO);

			
			cv::Canny(src_bin, src_canny, param1.houghParam1 * param1.houghRate, param1.houghParam2 * param1.houghRate);
			std::vector<cv::Vec3f> circles;
			cv::HoughCircles(src_bin, circles, cv::HOUGH_GRADIENT,
				param1.houghDp,
				param1.houghMinDist * param1.houghRate,
				param1.houghParam1 * param1.houghRate,
				param1.houghParam2 * param1.houghRate,
				param1.houghRadius - RATE_R, param1.houghRadius + RATE_R);
			
			int count = circles.size();
			
			int* string = new int[count];
			for (int i = 0; i < count; i++) {
				string[i] = 0;
			}

			if (count > 1&&param1.switchString==1) {
				CircleDistance(circles, string, width, height);
			}
			

			printf("Count::%d\n", count);
			int countx2 = 0;

			if (count < 1000) {
				for (int i = 0; i < count; i++) {
					if (string[i] == 1 && param1.switchString == 1) {
						cv::circle(src_clone, cv::Point(circles[i][0], circles[i][1]), circles[i][2], cv::Scalar(0, 255, 255), 5, cv::LINE_AA);
						countx2++;
					}
					else if (param1.switchString == 0) {
						cv::circle(src_clone, cv::Point(circles[i][0], circles[i][1]), circles[i][2], cv::Scalar(0, 255, 255), 5, cv::LINE_AA);
					}
					
				}
				if ( param1.switchString == 1) {
					count = countx2;
				}
			}
			src_clone = AddCount(src_clone, count);

			cv::imwrite("img.png", src_clone);
			cv::imshow("bin", src_canny);
			cv::imshow("Hough", src_clone);

			cv::resizeWindow("Hough", 640, 480);

			pam2 = pam1;
			delete[] string;
		}
		cv::waitKey(5);
	}

	return 0;
}

cv::Mat PrepareImg(cv::Mat dst) {
	cv::VideoCapture cap(0);//デバイスの番号


	if (!cap.isOpened()) {
		return dst;
	}

	int radius = 50;
	int Capture = 0;

	cv::namedWindow("Trackbar");
	cv::createTrackbar("Radius", "Trackbar", &param1.houghRadius, 70);
	cv::createTrackbar("Capture", "Trackbar", &Capture, 1);
	cv::Mat frame;
	cv::Mat img;


	while (cap.read(frame)) {
		int width = frame.cols;
		int height = frame.rows;
		img = frame.clone();
		cv::circle(img, cv::Point(width / 2, height / 2), param1.houghRadius, cv::Scalar(0, 255, 255), 5, cv::LINE_AA);
		cv::imshow("win", img);
		const int key = cv::waitKey(1);
		if (key == 'q') {
			break;
		}
		else if (Capture==1) {
			dst = frame.clone();
			break;
		}
	}
	cv::destroyAllWindows();
	return dst;

}

cv::Mat AddCount(cv::Mat dst, int count) {

	char str[10];

	cv::rectangle(dst, cv::Point(0, 0), cv::Point(100, 50), cv::Scalar(0, 255, 255), -1);
	
	sprintf_s(str, "%d", count);
	std::string num(str);

	cv::putText(dst, num, cv::Point(0, 25), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255));

	return dst;
}

void ChangeHoughParam1(int pos, void* userdata) {
	printf("param1.houghParam1 %d\n", param1.houghParam1);
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

void CallbackButton(int state, void* userdata) {

}

void ChangeImg(int pos, void* userdata) {
	param1.houghMinDist = 2 * param1.houghRadius;
}

void CircleDistance(std::vector<cv::Vec3f> circle, int* string,int width,int height) {
	int circle_size = circle.size();
	int center_x = 0, center_y = 0, point = 0;
	int minPoint = 100000;
	int centerNum = 0;
	int secondCenterNum = 0;
	int distance_x = 0,distance_y=0,distance=0;
	int radiusX2 = 0;
	int group = 1;
	for (int i = 0; i < circle.size(); i++) {
		center_x = (int)abs(width / 2 - circle[i][0]);
		center_y = (int)abs(width / 2 - circle[i][1]);
		point = center_x * center_x + center_y * center_y;
		if (minPoint > point) {
			secondCenterNum = centerNum;
			centerNum = i;
			minPoint = point;
		}
	}
	distance_x = (int)abs(circle[secondCenterNum][0] - circle[centerNum][0]);
	distance_y = (int)abs(circle[secondCenterNum][1] - circle[centerNum][1]);
	radiusX2 = distance_x * distance_x + distance_y * distance_y;

	string[centerNum] = 1;
	string[secondCenterNum] = 1;

	for (int i = 0; i < circle_size; i++) {
		for (int j = 0; j < circle_size; j++) {
			distance_x = (int)abs(circle[i][0] - circle[j][0]);
			distance_y = (int)abs(circle[i][1] - circle[j][1]);
			distance = distance_x * distance_x + distance_y * distance_y;
			if (RATE_D*radiusX2 > distance) {
				if (string[i] == 0 && string[j] == 0) {
					string[i] = group + 1;
					string[j] = group + 1;
					group++;
				}
				else if (string[i] == 1 || string[j] == 1) {
					string[i] = 1;
					string[j] = 1;
				}
				else if (string[i] >1&&string[j]>1) {
					if (string[i] > string[j]) {
						string[i] = string[j];
					}
					else {
						string[j] = string[i];
					}
				}
				else {
					if (string[i] == 0) {
						string[i] = string[j];
					}
					else {
						string[j] = string[i];
					}
				}
			}
		}
	}
	for (int k = 0; k < circle_size; k++) {
		printf("%d ",string[k]);
	}
}