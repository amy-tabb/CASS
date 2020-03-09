//============================================================================
// Name        : aruco-pattern-read-write-project.cpp
// Author      : Amy Tabb
// Version     :
// Copyright   : MIT
// Description :
//============================================================================


#include <sys/stat.h>
#include <sys/time.h>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <sys/stat.h>
#include <sys/time.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>
#include <chrono>
#include <getopt.h>


using namespace std;
using namespace cv;


template<class T>
T FromString(const std::string& s)
{
	std::istringstream stream (s);
	T t;
	stream >> t;
	return t;
}

template<class T>
string ToString(T arg)
{
	std::ostringstream s;

	s << arg;

	return s.str();

}


void EnsureDirHasTrailingBackslash(string& write_directory){
	int n_letters = write_directory.size();
	bool eval =  (write_directory[n_letters - 1] == '/');
	cout << "Last character compare " << write_directory << " " <<  eval << endl;
	if (eval == false){
		write_directory = write_directory + "/";
	}

}


int Create(string read_file, string write_directory){
	/// mainCreate creates the backstop, some in as well as the individual markers.

	Size imageSize;
	int squaresX = 12;
	int squaresY = int(float(squaresX)*1.25);
	int squareLength = 200;
	int markerLength = 100;
	int margins = squareLength - markerLength;
	int arc_code =  cv::aruco::DICT_6X6_1000;
	int white_space_buff = squareLength - markerLength;
	int extra_to_add = 2*margins - white_space_buff;

	string temp_string;

	ifstream in;
	in.open(read_file.c_str());

	in >> temp_string >> squaresX;
	in >> temp_string >> squaresY;
	in >> temp_string >> squareLength;
	in >> temp_string >> markerLength;
	in >> temp_string >> margins;
	in >> temp_string >> arc_code;

	in.close();


	std::ofstream out;
	string filename = write_directory + "specification_file.txt";
	out.open(filename.c_str());

	Ptr<aruco::Dictionary> dictionary =
			aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(arc_code));

	out << "squaresX " << squaresX << endl;
	out << "squaresY " << squaresY << endl;
	out << "squareLength " << squareLength << endl;
	out << "markerLength " << markerLength << endl;
	out << "margins " << margins << endl;
	out << "arc_code " << arc_code << endl;
	out.close();


	white_space_buff = squareLength - markerLength;
	extra_to_add = 2*margins - white_space_buff;

	imageSize.width = squaresX * squareLength + extra_to_add;
	imageSize.height = squaresY * squareLength + extra_to_add;

	Mat markerImg;
	Mat boardImage = Mat::zeros(imageSize.height, imageSize.width, CV_8UC1);
	boardImage.setTo(255);
	int x0, y0;

	for (int x = 0, count = 0; x < squaresX; x++){
		for (int y = 0; y < squaresY; y++, count++){

			aruco::drawMarker(dictionary, count, markerLength, markerImg, 1);

			/// where to place?
			x0 = x*squareLength + margins;
			y0 = y*squareLength + margins;

			Rect R = Rect(x0, y0, markerLength, markerLength);


			markerImg.copyTo(boardImage(R));
		}
	}


	filename = write_directory + "aruco_image" + ToString<int>(squaresX) + "by" + ToString<int>(squaresY) + ".png";
	cv::imwrite(filename, boardImage);


	return 0;
}

bool CheckExistenceOfDirectory(string write_directory){

	bool exists= true;
	struct stat info;
	if( stat( write_directory.c_str(), &info ) != 0 ){
		cout << "Path to directory is wrong and/or cannot access " << write_directory << endl;
		exists = false;
	}

	return exists;

}

int main(int argc, char **argv){

	string write_directory = "";
	string input_file = "";

	int print_help = 0;

	if (argc == 1){
		print_help = 1;
	}

	while (1)
	{
		static struct option long_options[] =
		{
				{"help",   no_argument,       &print_help, 1},
				/* These options don’t set a flag.
			             We distinguish them by their indices. */
				{"input",   required_argument, 0, 'a'},
				{"output",   required_argument, 0, 'b'},
		};


		if (print_help == 1){
			cout << "Printing help for aruco-pattern-write" << endl;

			cout << "OPTIONAL FLAGS WITHOUT ARGUMENT -------------------" << endl;
			cout << std::left << setw(30) << "--help" << "No arguments.  Prints this help information." << endl;

			cout << endl;
			cout << "DIRECTORIES AND PATHS ----------------------- " << endl;
			cout << std::left << setw(30) << "--input=[STRING] " << "Mandatory, has to be a file specifying the pattern." << endl;
			cout << std::left << setw(30) << "--output=[STRING] " << "Mandatory, has to be a directory." << endl;

			exit(1);
		}
		/* getopt_long stores the option index here. */
		int option_index = 0;
		int opt_argument;

		opt_argument = getopt_long (argc, argv, "ab",
				long_options, &option_index);

		/* Detect the end of the options. */
		if (opt_argument == -1)
			break;

		switch (opt_argument)
		{
		case 0:
			if (long_options[option_index].flag != 0)
				break;
			printf ("option %s", long_options[option_index].name);
			if (optarg)
				printf (" with arg %s", optarg);
			printf ("\n");
			break;

		case 'a':
			input_file = optarg;
			break;
		case 'b':
			write_directory = optarg;
			break;

		}

	}

	if (input_file.size() == 0){
		cout << "Please enter an input file --input=[STRING] argument.  Use --help to see the options." << endl; exit(1);
	}

	if (write_directory.size() == 0){
		cout << "Please enter a write directory --input=[STRING] argument.  Use --help to see the options." << endl; exit(1);
	}

	ifstream in;
	in.open(input_file.c_str());
	if (!in.good())
	{
		cout << "Input file entered is bad. " << input_file << endl; exit(1);
	}
	in.close();


	if (!CheckExistenceOfDirectory(write_directory)){
		exit(1);
	}


	EnsureDirHasTrailingBackslash(write_directory);

	Create(input_file, write_directory);

	cout << "Pattern create completed!  Check the directory for the image file" << endl;

	exit(1);

}






