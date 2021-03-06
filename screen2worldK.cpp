#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "screen2worldK.h"

// In order to instantiate the converter, the camera_matrix
// and the points correspondences can be used. This constructor
// will calculate the rotation and translation vectors. See the
// other constructor, which can directly load the matrix.
Screen2World::Screen2World(char* camMatrix, char* correspondences) {
	init(camMatrix, correspondences);
}

// Implements the initialization
void Screen2World::init(char* camMatrix, char* correspondences){
	// Get K and dC. To generate it, use
	// https://github.com/rodolfoap/cameracalibration
	fs.open(camMatrix, cv::FileStorage::READ);
	fs["camera_matrix"] >> K;
	fs["distortion_coefficients"] >> dC;
	fs.release();

	// Get screen & world points. To generate them, use
	// https://github.com/rodolfoap/points-picker
	fs.open(correspondences, cv::FileStorage::READ);
	fs["image_points"] >> screenpts;
	fs["world_points"] >> worldpts;
	fs.release();

	// The solvePnP() method requires a set of screen and world points correspondences,
	// as well as the camera matrix and distortion coefficients. With such information,
	// the rotation vector and the translation vector of the camera are generated.
	cv::solvePnP(worldpts, screenpts, K, dC, rvec, tvec);
	Rodrigues(rvec, R);

	// Camera position is calculated with this:
	pos=-R.t()*tvec;

	// Generate yaml file
	fs.open("rotation_translation_matrix.yaml", cv::FileStorage::WRITE);
	fs<<"rotation_matrix"<<R;
	fs<<"rotation_vector"<<rvec;
	fs<<"translation_vector"<<tvec;
	fs<<"camera_position"<<pos;
	fs.release();

	if(DEBUG) {
		std::cerr<<std::endl<<"Camera Matrix:"<<std::endl<<K<<std::endl;
		std::cerr<<std::endl<<"Distortion Coefficients:"<<std::endl<<dC<<std::endl;
		std::cerr<<std::endl<<"Image Points:"<<std::endl<<screenpts<<std::endl;
		std::cerr<<std::endl<<"World Points:"<<std::endl<<worldpts<<std::endl;
		std::cerr<<std::endl<<"RVec:"<<std::endl<<rvec<<std::endl;
		std::cerr<<std::endl<<"RMat:"<<std::endl<<R<<std::endl;
		std::cerr<<std::endl<<"TVec:"<<std::endl<<tvec<<std::endl<<std::endl;
		std::cerr<<std::endl<<"CPos:"<<std::endl<<pos<<std::endl<<std::endl;
	}
}

// This alternative constructor will simply load the rotation
// and translation vectors. See the other constructor, which
// will perform the full calculation by loading the camera matrix
// and a set of world-screen point correspondences.
Screen2World::Screen2World(char* rtMatrix) {
	// Get K and dC. To generate it, use
	// https://github.com/rodolfoap/cameracalibration
	fs.open(rtMatrix, cv::FileStorage::READ);
	fs["rotation_matrix"] >> R;
	fs["rotation_vector"] >> rvec;
	fs["translation_vector"] >> tvec;
	fs["camera_position"] >> pos;
	if(DEBUG) {
		std::cerr<<std::endl<<"RVec:"<<std::endl<<rvec<<std::endl;
		std::cerr<<std::endl<<"RMat:"<<std::endl<<R<<std::endl;
		std::cerr<<std::endl<<"TVec:"<<std::endl<<tvec<<std::endl<<std::endl;
		std::cerr<<std::endl<<"CPos:"<<std::endl<<pos<<std::endl<<std::endl;
	}
	fs.release();
}

cv::Point3d Screen2World::getWorldCoords(double u, double v){
	// Having prepared the context for the transformation, a screen point (srcPoint)
	// is created so its world coordinates should be obtained
	cv::Mat uv1=cv::Mat::ones(3, 1, cv::DataType<double>::type);
	uv1.at<double>(0, 0)=u;
	uv1.at<double>(1, 0)=v;
	// Last row uv1(2, 0) is 1

	// Hypothesis ground:
	double Z=0;

	//  |u|       |X|
	// s|v| = K (R|Y| + t)             (1)
	//  |1|       |Z|             (Z is 0)
	//
	// and therefore,
	//
	// |X|    -1  -1  |u|      -1
	// |Y| = R   K   s|v| - t R        (2)
	// |Z|            |1|
	cv::Mat invR_invK_uv1= R.inv() * K.inv() * uv1; // first  term on (2)
	cv::Mat invR_tvec    = R.inv() * tvec;          // second term on (2)

	// |X|    |.|   |.|          z+b
	// |Y| = s|.| - |.|, so  s = ---   (3)
	// |Z|    |a|   |b|           a
	double s=(Z+invR_tvec.at<double>(2, 0)) / invR_invK_uv1.at<double>(2, 0);

	// Having s, the XYZ (Z is 0) vector can be calculated from (3)
	cv::Mat XYZ0=s * invR_invK_uv1 - invR_tvec;

	if(DEBUG) {
		std::cerr<<"Camera Coordinates:"<<std::endl<<uv1<<std::endl<<std::endl;
		std::cerr<<"World Coordinates:"<<std::endl<<XYZ0<<std::endl<<std::endl;
		std::cout<<"["  << uv1.at<double>(0, 0) << ", "
				<< uv1.at<double>(1, 0) << "] > ["
				<< XYZ0.at<double>(0)   << ", "
				<< XYZ0.at<double>(1)   << ", 0]"
				<< std::endl;
	}

	// Simpler format to return the point
	cv::Point3d XYZ(XYZ0.at<double>(0, 0), XYZ0.at<double>(1, 0), XYZ0.at<double>(2, 0));
	return XYZ;
}
