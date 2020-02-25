//============================================================================
// Name        : camera-as-scanner.cpp
// Author      : Amy Tabb
// Version     :
// Copyright   : MIT
// Description :
//============================================================================


#include "Includes.hpp"
#include "camera_calibration.hpp"
#include <getopt.h>
#include <iostream>

using namespace std;



void EnsureDirHasTrailingBackslash(string& write_directory){
	int n_letters = write_directory.size();
	bool eval =  (write_directory[n_letters - 1] == '/');
	cout << "Last character compare " << write_directory << " " <<  eval << endl;
	if (eval == false){
		write_directory = write_directory + "/";
	}

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

int main(int argc, char **argv) {


	string read_directory = "";
	string write_directory = "";
	string image_filename = "";
	double homography_scaling = 0;

	int write_intermediate = false;

	int print_help = 0;

	while (1)
	{
		static struct option long_options[] =
		{
				{"help",   no_argument,       &print_help, 1},
				{"write-inter", no_argument,       &write_intermediate, 1},
				/* These options don’t set a flag.
		             We distinguish them by their indices. */
				{"input",   required_argument, 0, 'a'},
				{"output",  required_argument, 0, 'b'},
				{"px-per-mm", required_argument, 0, 'c'},
		};


		if (print_help == 1){
			cout << "Printing help for camera-as-scanner" << endl;

			cout << "OPTIONAL FLAGS WITHOUT ARGUMENT -------------------" << endl;
			cout << std::left << setw(30) << "--help" << "No arguments.  Prints this help information." << endl;
			cout << std::left << setw(30) << "--write-inter" << "No arguments, write intermediate information." << endl;

			cout << endl;
			cout << "DIRECTORIES AND PATHS ----------------------- " << endl;
			cout << std::left << setw(30) << "--input=[STRING] "<< "Mandatory, has to be a directory." << endl;
			cout << std::left << setw(30) << "--output=[STRING] " << "Mandatory, has to be a directory." << endl;
			cout << std::left << setw(30) << "--px-per-mm=[float] " << "Scaling, number of pixels per millimeter in the result images " << endl;


			exit(1);
		}
		/* getopt_long stores the option index here. */
		int option_index = 0;
		int opt_argument;

		opt_argument = getopt_long (argc, argv, "abc",
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
			read_directory = optarg;
			break;
		case 'b':
			write_directory = optarg;

			break;
		case 'c':
			string temp = optarg;
			if (temp.size() == 0){
				cout << "Please enter a scaling --px-per-mm= argument.  Use --help to see the options." << endl; exit(1);
			}
			homography_scaling = FromString<float>(optarg);
			break;

		}

	}

	if (read_directory.size() == 0){
		cout << "Please enter a read directory --input=[STRING] argument.  Use --help to see the options." << endl; exit(1);
	}

	if (write_directory.size() == 0){
		cout << "Please enter a write directory --output=[STRING] argument.  Use --help to see the options." << endl; exit(1);
	}

	if (homography_scaling == 0){
		cout << "Please enter a scaling --px-per-mm= argument that is > 0.  Use --help to see the options." << endl; exit(1);
	}

	/// check that both directories exist first.
	if (!CheckExistenceOfDirectory(read_directory)){
		exit(1);
	}

	if (!CheckExistenceOfDirectory(write_directory)){
		exit(1);
	}

	EnsureDirHasTrailingBackslash(read_directory);
	EnsureDirHasTrailingBackslash(write_directory);


	PatternsCreated P_Class(read_directory, write_directory, true);

	string id_directory = read_directory + "images/";

	cout << "Setting up camera calibration class ... " << endl;
	CameraCali* C = new CameraCali(id_directory, &P_Class);

	cout << endl << "Reading EXIF information for all images " << endl;
	C->ReadExifInformationForAllImages(id_directory, read_directory, write_directory);
	cout << endl << "Finding aruco patterns in each image " << endl;
	C->FindCornersArucoGeneral(write_directory, write_intermediate);

	cout << endl << "Calibrating each image and warping." << endl;
	C->CalibrateArucoSinglyAndUndistort(write_directory, homography_scaling, write_intermediate);


	cout << endl << "The code run is now complete!  Processed images are prefixed with warped. " << endl;
	delete C;

	return 0;
}


