/*
 * camera_calibration.hpp
 *
 *  Created on: Jun 26, 2018
 *      Author: atabb
 */

#ifndef CAMERA_CALIBRATION_HPP_
#define CAMERA_CALIBRATION_HPP_

#include <iostream>


#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <inttypes.h>
#include <parallel/algorithm>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#include <omp.h>


#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

#include "Includes.hpp"


using namespace Eigen;
using namespace cv;
using std::vector;
using std::ifstream;
using std::string;


using std::set;
using std::max;
using std::min;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::list;
using std::vector;
using std::map;
using std::pair;

// double to single mapping -- take 0 9 to number

class PatternsCreated{
public:
	vector<vector<int> > double_to_single;
	vector< cv::Point3f> three_d_points;
	vector< cv::Point3f> three_d_points_internal;
	Ptr<aruco::Dictionary> dictionary;
	vector< vector<int> > display_colors;
	vector<pair<int, int> > min_max_id_pattern;
	vector<pair<int, int> > min_max_id_squares;
	bool single_aruco_markers;
	vector<int> single_aruco_ids;
	int max_internal_patterns;
	int internalx, internaly;

	PatternsCreated(string read_dir, string write_directory, bool aruco_markers);

	int MappingArucoIDToPatternNumber(int id);

	Scalar Color(int index);

	int NumberMarkers();
	int NumberSquares();
	int NumberPatterns();

	void SetNumberMarkers(int n);
	void SetNumberPatterns(int n);

protected:
	int number_patterns;
	int number_total_squares;
	int int_number_markers;

};


class CameraCali{
public:

	vector<vector<bool> > patterns_present; //aruco
	vector<MatrixXd> two_d_point_coordinates_dense;
	vector<vector<int> > points_per_board;
	vector<vector<bool> > boards_detected;
	vector< vector<double> > reproj_error_per_board;

	vector<vector<bool> > has_calibration_estimate;
	vector<string> im_names;
	vector<string> im_short_names;

	vector<MatrixXd> internal_two_d_point_coordinates_dense;
	int count_internal_ids_present;
	vector<bool> id_bool;
	vector<double> focal_lengths;
	vector<double> pix_widths;
	double sensor_width;
	double pixel_width;
	int rows;
	int cols;

	vector<Mat> images; // the calibration images in the set
	int number_external_images_max;
	vector<Mat> reproject_cam_cali_images;

	Matrix3d internal_parameters;
	VectorXd distortion;
	vector<vector< Matrix4d> > external_parameters;

	PatternsCreated* P_class;

	vector<int> internal_pattern_indices;
	vector<MatrixXd> two_d_point_coordinates_segmentation_external;
	vector<MatrixXd> two_d_point_coordinates_segmentation_internal;

	CameraCali(string read_dir, PatternsCreated* P);

	void ReadExifInformationForAllImages(string image_read_dir, string parent_dir, string write_directory);

	void CopyToMats(Mat& CameraMatrix, Mat& dis);

	void FindCornersArucoGeneral(string write_dir, bool write_intermediate);

	void CalibrateArucoSinglyAndUndistort(string write_dir, double homograph_scaling, bool write_intermediate);

};


#endif /* CAMERA_CALIBRATION_HPP_ */
