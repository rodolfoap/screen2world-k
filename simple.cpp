#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "screen2worldK.h"
#define LOG std::cerr<<">>> "<<__FILE__<<"["<<__LINE__<<"]:"<<__func__<<"();"<<std::endl;

int main(int argc, char** args) {
	if(argc<3) { std::cerr<<"Usage: "<<args[0]<<" [CAMERA_MATRIX.yaml POINTS_FILE.yaml]"<<std::endl; exit(1); }
	Screen2World s2w(args[1], args[2]);
	cv::Point3d point=s2w.getWorldCoords(500.0, 482.0);
	std::cerr<<"Result: (500, 428) > "<<point<<std::endl;
	return 0;
}
