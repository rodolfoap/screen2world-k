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
		oimage.copyTo(image);
		cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
		cv::imshow("Image", image);
		cv::setMouseCallback("Image", onMouse);
		cv::waitKey();
	}

	static void onMouse(int event, int x, int y, int, void*) {
		static cv::Point pold, pnew;
		static bool active=false;
		oimage.copyTo(image);
		cv::Point3d point=s2w.getWorldCoords(x, y);
    		cv::putText(	image, "Hello", cv::Point(10, 10),
				cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0,200,250), 1, cv::LINE_AA);

		// See enum cv::MouseEventTypes
		if(event==cv::EVENT_LBUTTONDOWN) active=true;
		if(event==cv::EVENT_LBUTTONUP  ) active=false;
		if(active) {
			std::cerr<<"("<<x<<":"<<y<<")>"<<point<<std::endl;
			pnew=cv::Point(x, y);
			if(pold.x>0&&pold.y>0)  cv::line(image, pold, pnew, cv::Scalar(0, 255, 0), 1);
			else                    cv::line(image, pnew, pnew, cv::Scalar(0, 0, 255), 2);
			cv::imshow("Image", image);
			pold=pnew;
		}

	}
};

Screen2World Scene::s2w;
cv::Mat Scene::image, Scene::oimage;
int main(int argc, char** args) {
	if(argc<4) { std::cerr<<"Usage: "<<args[0]<<" [CAMERA_MATRIX.yaml POINTS_FILE.yaml IMAGE_FILE]"<<std::endl; exit(1); }
	Scene s(args[1], args[2], args[3]);
	return 0;
}
