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
	int houghDp = 0;
	int houghMinDist = 0;
	int houghParam1 = 0;
	int houghParam2 = 0;
	int houghMinRadius = 0;
	int houghMaxRadius = 0;
	int updateKey = 0;
	int houghRate = 0;

};

CountParam param1;

cv::Mat src_gray;
cv::Mat src_bin;
cv::Mat src_canny;
cv::Mat src;

int pam1 = 0;
int pam2 = 1;

int main() {

	src = cv::imread("C:\\Users\\isoff\\WORK_SPACE\\ConsoleApplicationSample\\OSAKAPICTURE\\img3.png");
	if (src.empty() == true) {
		return -1;
	}
	int width = src.cols;
	int height = src.rows;
	cv::Mat src_clone = src.clone();
	
	param1.houghDp = 1;
	param1.houghMinDist = 19;
	param1.houghParam1 = 100;
	param1.houghParam2 = 15;
	param1.updateKey = 0;
	param1.houghRate = 1;


	int filter_prewitt[3][3] = {
		{-1,0,1},
		{-1,0,1},
		{-1,0,1}
	};
	int filter_size = 3;
	int filter_size2 = filter_size / 2;
	cv::namedWindow("Hough", cv::WINDOW_NORMAL);
	cv::namedWindow("bin", cv::WINDOW_NORMAL);
	cv::setWindowProperty("Hough", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FREERATIO);
	cv::setWindowProperty("bin", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FREERATIO);

	cv::namedWindow("Trackbar");
	cv::createTrackbar("Dp", "Trackbar", &param1.houghDp, 5, ChangeHoughParam1);
	cv::createTrackbar("MinDist", "Trackbar", &param1.houghMinDist, 100, ChangeHoughParam1);
	cv::createTrackbar("param1", "Trackbar", &param1.houghParam1, 100, ChangeHoughParam1);
	cv::createTrackbar("param2", "Trackbar", &param1.houghParam2, 100, ChangeHoughParam1);
	cv::createTrackbar("houghRate", "Trackbar", &param1.houghRate, 4, ChangeHough);
	cv::createTrackbar("update", "Trackbar", &param1.updateKey, 1, ChangeHough);

	
	cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(src_gray, src_bin, cv::Size(5, 5), 0);
	cv::Canny(src_bin,src_canny,param1.houghParam1* param1.houghRate,param1.houghParam2* param1.houghRate);

	//threshold(src_gray, src_bin, param1.thresh, param1.maxval, cv::THRESH_BINARY);
	/*src_bin = src_gray.clone();
	for (int y = filter_size2; y < height - filter_size2; y++) {
		for (int x = filter_size2; x < width - filter_size2; x++) {
			int val = 0;
			for (int n = -filter_size2; n <= filter_size2; n++) {
				for (int m = -filter_size2; m <= filter_size2; m++) {
					val += src_gray.at<unsigned char>(y + n, x + m) * filter_prewitt[m + filter_size2][n + filter_size2];
				}
			}
			if (val > 255)
				val = 255;
			if (val < 0)
				val = 0;
			src_bin.at<unsigned char>(y, x) = val;
		}
	}*/

	cv::imshow("bin", src_canny);
	cv::resizeWindow("bin", 640, 480);

	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(src_bin, circles, cv::HOUGH_GRADIENT,
		param1.houghDp,
		param1.houghMinDist * param1.houghRate,
		param1.houghParam1 * param1.houghRate,
		param1.houghParam2 * param1.houghRate,
		10, 30);

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

			//threshold(src_gray, src_bin, param1.thresh, param1.maxval, cv::THRESH_BINARY);
			/*src_bin = src_gray.clone();
			for (int y = filter_size2; y < height - filter_size2; y++) {
				for (int x = filter_size2; x < width - filter_size2; x++) {
					int val = 0;
					for (int n = -filter_size2; n <= filter_size2; n++) {
						for (int m = -filter_size2; m <= filter_size2; m++) {
							val += src_gray.at<unsigned char>(y + n, x + m) * filter_prewitt[m + filter_size2][n + filter_size2];
						}
					}
					if (val > 255)
						val = 255;
					if (val < 0)
						val = 0;
					src_bin.at<unsigned char>(y, x) = val;
				}
			}*/
			cv::Canny(src_bin, src_canny, param1.houghParam1* param1.houghRate, param1.houghParam2* param1.houghRate);
			std::vector<cv::Vec3f> circles;
			cv::HoughCircles(src_bin, circles, cv::HOUGH_GRADIENT,
				param1.houghDp,
				param1.houghMinDist * param1.houghRate,
				param1.houghParam1 * param1.houghRate,
				param1.houghParam2 * param1.houghRate,
				10, 30);

			int count = circles.size();
			printf("Count::%d\n", count);
			
			if (count < 1000) {
				for (int i = 0; i < count; i++) {
					cv::circle(src_clone, cv::Point(circles[i][0], circles[i][1]), circles[i][2], cv::Scalar(0, 255, 255), 5, cv::LINE_AA);
				}
			}
			
			cv::imwrite("img.png", src_clone);
			cv::imshow("bin", src_canny);
			cv::imshow("Hough", src_clone);

			cv::resizeWindow("Hough", 640, 480);

			pam2 = pam1;
		}
		cv::waitKey(5);
	}

	return 0;
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