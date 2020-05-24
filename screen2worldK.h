#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class Screen2World {
private:
	cv::FileStorage fs;
	cv::Mat K=cv::Mat::eye(3, 3, CV_64F);
	cv::Mat dC=cv::Mat::zeros(4, 1, CV_64F);
	cv::Mat_<cv::Point2d> screenpts;
	cv::Mat_<cv::Point3d> worldpts;
	cv::Mat rvec, tvec, R, pos;
public:
	// In order to instantiate the converter, the camera_matrix
	// and the points correspondences can be used. This constructor
	// will calculate the rotation and translation vectors. See the
	// other constructor, which can directly load the matrix.
	Screen2World(char* camMatrix, char* correspondences);

	// This alternative constructor will simply load the rotation
	// and translation vectors. See the other constructor, which
	// will perform the full calculation by loading the camera matrix
	// and a set of world-screen point correspondences.
	Screen2World(char* rtMatrix);

	// The conversion method
	cv::Point3d getWorldCoords(double u, double v);
};
