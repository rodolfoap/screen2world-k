#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "screen2worldK.h"
#define LOG std::cerr<<">>> "<<__FILE__<<"["<<__LINE__<<"]:"<<__func__<<"();"<<std::endl;

class Scene {
public:
	static cv::Mat image, oimage;
	static Screen2World s2w;
	Scene(char* cam, char* points, char* imgfile) {
		s2w.init(cam, points);
		oimage=cv::imread(imgfile, cv::IMREAD_COLOR);
		cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
		cv::imshow("Image", oimage);
		cv::setMouseCallback("Image", onMouse);
		cv::waitKey();
	}

	static void onMouse(int event, int x, int y, int, void*) {
		static cv::Point pold, pnew;
		static bool active=false;
		oimage.copyTo(image);
		cv::Point3d point=s2w.getWorldCoords(x, y);
		cv::String text=cv::format("Coords: [%.6f, %.6f, %.6f]", point.x, point.y, point.z);
		cv::rectangle(image, cv::Point(0, 0), cv::Point(900, 60), cv::Scalar(0, 0, 0), -1, 8, 1);
    		cv::putText(	image, text, cv::Point(20, 20),
				cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
		cv::imshow("Image", image);

	}
};

Screen2World Scene::s2w;
cv::Mat Scene::image, Scene::oimage;
int main(int argc, char** args) {
	if(argc<4) { std::cerr<<"Usage: "<<args[0]<<" [CAMERA_MATRIX.yaml POINTS_FILE.yaml IMAGE_FILE]"<<std::endl; exit(1); }
	Scene s(args[1], args[2], args[3]);
	return 0;
}
