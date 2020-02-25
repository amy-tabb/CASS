# CASS

Camera-as-Scanner: Code for taking measurements from images of an object on top of a calibration pattern.

Roadmap
- [Contact](#contact)
- [References](#underlying-ideas-how-and-when-to-cite-this-work)
- [Dependences](#dependencies)
	- [Tested operating system](#tested-operating-system)
	- [OpenCV 4](#opencv-4)
	- [Exiftools](#exiftools)
- [Building](#building)
- [Running camera-as-scanner executable](running-camera-as-scanner-executable)
	- [Read directory format, camera-as-scanner](read-directory-format-camera-as-scanner)
	- [Write directory format, camera-as-scanner](write-directory-format-camera-as-scanner)
- [Running aruco-pattern-write executable](#running-aruco-pattern-write-executable)
	- [Directory format, input aruco-pattern-write ](#directory-format-input-aruco-pattern-write)
	- [Directory format, output aruco-pattern-write ](#directory-format-output-aruco-pattern-write)
	- [Altering](#altering)

#Contact

Comments/Bugs/Problems: amy.tabb@usda.gov

Code for taking measurements from images of an object on top of a calibration pattern.

March 2019.
- update March 2020.  

TODO -- DOI for code.

# Underlying ideas; how and when to cite this work

Computer vision explanation: This code takes an image of an object on top of an aruco calibration pattern, calibrates the camera using the detected aruco information as well as EXIF tag information, and undistorts and computes the homography from the current location of the aruco calibration pattern in the image to its location in physical space.  Then the image is warped to match the coordinate system of the aruco coordinate system, scaled by a user-selected parameter. We abbreviate this method as CAmera aS Scanner, or CASS.

This README file is to accompany code produced by Amy Tabb as a companion to a paper.  The paper, currently in pre-print, provides a full protocol to use the code:

[Using cameras for precise measurement of two-dimensional plant features: CASS (arXiv)](https://arxiv.org/abs/1904.13187)

Paper citation:
````latex
@article{tabb_using_2019,
	title = {Using cameras for precise measurement of two-dimensional plant features: CASS},
	url = {https://arxiv.org/abs/1904.13187v1},
	urldate = {2019-05-02},
	author = {Tabb, Amy and Holguín, Germán A. and Naegele, Rachel},
	month = apr,
	year = {2019},
}
````

Code release citation:
Tabb, Amy. (2020). Data and Code from: Using cameras for precise measurement of two-dimensional plant features: CASS (Version v 1.0) [Data set]. Zenodo. http://doi.org/10.5281/zenodo.3677473

````latex
@dataset{tabb_amy_2020_3677473,
  author       = {Tabb, Amy},
  title        = {{Data and Code from: Using cameras for precise measurement of two-dimensional plant features: CASS}},
  month        = feb,
  year         = 2020,
  publisher    = {Zenodo},
  version      = {v 1.0},
  doi          = {10.5281/zenodo.3677473},
  url          = {https://doi.org/10.5281/zenodo.3677473}
}
````


If you use this code in project that results in a publication, please cite at a minimum the paper above.  Otherwise, there are no restrictions in your use of this code.  However, no guarantees are expressed or implied.

## Dependencies

This code uses the OpenCV 4.0, OpenCV 4.0 extra modules, Eigen and is written in C++. It also uses the `exiftool` executable.

### Tested operating system

This code has been tested on Ubuntu 16.04 and Ubuntu 18.04.  You are welcome to convert it to Windows, but I have not.  While OpenCV is available from distribution repositories, my long experience with it is has always been to build from the source to get the best results.

### OpenCV 4


To get the OpenCV 4.xx extra modules to build, our experience is that you need to build *both* OpenCV and the extra modules together from source.  Instructions are here:

[OpenCV contributed modules on Github](https://github.com/opencv/opencv_contrib)

These libraries need to be installed: 

- `opencv_core`
- `opencv_imgproc`
- `opencv_imgcodecs` 
- `opencv_aruco` 
- `opencv_calib3d`


### Exiftools

[exiftool](https://owl.phy.queensu.ca/~phil/exiftool/) On Ubuntu, install `exiftool` as follows:

```
sudo apt-get install exiftool
```

or

```
sudo apt-get install libimage-exiftool-perl
```

To test, make sure that the `exiftool` can be run from directory where you are setting up the `camera-as-scanner` project.

TODO -- Docker release to come.

## Building 

To build, you'll also need Cmake.  Alternate methods are available, but cmake is faster.  On Ubuntu, 

```` bash
sudo apt-get install cmake
````


1. Clone the git repository to a desired location.. 

```bash
git clone https://github.com/amy-tabb/CASS.git
```

Then change to the `CASS`.  
 
```bash
cd CASS
```

2. Create a `build` folder (or something similar), and `cd` into it: `cd build`.

3. Configure with cmake.  Don't have cmake? (`sudo apt-get install cmake`). Then from the build folder, you can use any of the following four options below: 

- `cmake ../src`  (basic)
- `cmake  -DCMAKE_BUILD_TYPE=Release ../src` (Release configuration)
- `cmake  -DCMAKE_BUILD_TYPE=Debug ../src` (Debug configuration)
- `cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE ../src/` (Create an Eclipse project to import -- it will be in the `build` folder)

In case you have installed OpenCV and cmake can't find it, you need to specify the location of `OpenCVConfig.cmake`.  Don't know where it is?   Find out with `locate OpenCVConfig.cmake`.  Then append

`-DCMAKE_PREFIX_PATH=dir`

in my case on one machine, this was:

`-DCMAKE_PREFIX_PATH=/usr/local/opencv41/lib/cmake/opencv4/`

where `/usr/local/opencv41/lib/cmake/opencv4/` is the directory containing `OpenCVConfig.cmake`.  Of course, you will substitute whatever the approrpriate directory returned from  `locate OpenCVConfig.cmake` was.

4. Then, you can either import the project to Eclipse (if you used the last option), and build from there, or type `make`.   If the everything compiled and linked, and you have two executables named `camera-as-scanner` and `aruco-pattern-write`, you are ready to go.

5. I highly suggest that you download at least one test dataset from [Zenodo data release](http://doi.org/10.5281/zenodo.3677473).  These datasets are in the format needed for CASS, and you can ensure that everything is correctly configured on your system.

### Alternates

If you want to use an alternate to cmake, the `camera-as-scanner` executable requires Eigen3 (a header-only library) and the following OpenCV libraries:

- opencv_core, opencv_highgui, opencv_imgproc, opencv_imgcodecs, opencv_calib3d, opencv_aruco.

And OpenCV 4.xx requires a C++11 compiler or higher.  

The `aruco-pattern-write` executable requires the following OpenCV libraries:

- opencv_core, opencv_imgcodecs, opencv_aruco.

The only file in the source folder needed for the `aruco-pattern-write` executable is the `aruco-pattern-write-project.cpp` file.  All of the `*.cpp` and `*.hpp` files are for the `camera-as-scanner` executable.


## Running camera-as-scanner executable

 Provided you're built the project using the provided cmake setup, you will get the `camera-as-scanner` executable. This program has three mandatory arguments and one optional one.  You can see the format by running with the `--help` flag.  

```bash
./camera-as-scanner --help

Printing help for camera-as-scanner
OPTIONAL FLAGS WITHOUT ARGUMENT -------------------
--help                        No arguments.  Prints this help information.
--write-inter                 No arguments, write intermediate information.

DIRECTORIES AND PATHS ----------------------- 
--input=[STRING]              Mandatory, has to be a directory.
--output=[STRING]             Mandatory, has to be a directory.
--px-per-mm=[float]           Scaling, number of pixels per millimeter in the result images
TODO -- redo all of this.  
```
The aguments are : 
- `--input` = read directory, 
- `--output` = write directory,
- `--px-per-mm` = the scaling factor (number of pixels per millimeter).

Optionally, you can write the intermediate outputs.  To do so, add flag `--write-iter`.  Our manuscript explains these items in detail.  If you're in a hurry, use 10 for the scaling factor.

An example valid run command is:

```
./camera-as-scanner --input ../../iphone6 --output ../../iphone6_results --px-per-mm 10 --write-inter

```

## Read directory format, camera-as-scanner

Examples of read and write directories are given within the [Zenodo data release](http://doi.org/10.5281/zenodo.3677473) repository.  A read and write directory is `iphone` and `iphone_results`, respectively.  A calibration pattern that can be used for printing is `iphone/created_template.png`, or one can use the companion executable `aruco-pattern-write` (usage explained [below](#running-aruco-pattern-write-executable)).  

- `calibration_object_info.txt` contains one line: `squarelength 25.5 mm` in the examples.  To find the correct vaue for squarelength, measure one square on the printed calibration pattern.  Edit the file appropriately.
- `sensor_size.txt` contains two lines: `sensor_width 4.80 mm`, `sensor_height 3.60 mm`.  Values for your camera can be found from the manufacturer's website.  Note that EXIFtag information is usually not accurate.
- `specification_file.txt` contains the information generated from companion executable `aruco-pattern-write`:

	```
	squaresX 12
	squaresY 15
	squareLength 200
	markerLength 100
	margins 100
	arc_code 11
	```

Note that if you use the pattern from the examples provided in this repository, you can copy the `specification_file.txt`.
-  `images` is the directory of image files.

## Write directory format, camera-as-scanner

For each image in the `images` directory, the program will produce the following file, where `FILENAME` is the original image filename:

- `aruco_detectFILENAME`: image with aruco tag detections overlaid (only if the optional intermediate write image file variable is true).
- `undistortedFILENAME`: image after undistortion opertion.  This is useful to inspect; if the calibration quality is poor, straight lines in reality will *not* be straight. (only if the optional intermediate write image flag is set `--write-inter`.)
- `warpedFILENAME`: image after applying transformation such that the image's coordinate system represents the calibration pattern's coordinate system, up to a scaling factor (as selected by the user -- last parameter). 

Text file `results.txt` records the scaling factor selected by the user.  From this, one can take two-dimensional measurements from the image (under the assumptions of planarity).  Say the scaling factor was 5, and a section is 50 pixels wide.  Assuming the units were millimeters and the object planar, the object is 10 mm wide.

`results.txt` also lists the calibration information for each image.


## Running aruco-pattern-write executable

The program takes two arguments: a specification file, and a write directory.

Like with `camera-as-scanner`, you can use `--help` for formatting:

```bash
 ./aruco-pattern-write --help
Printing help for aruco-pattern-write
OPTIONAL FLAGS WITHOUT ARGUMENT -------------------
--help                        No arguments.  Prints this help information.

DIRECTORIES AND PATHS ----------------------- 
--input=[STRING]              Mandatory, has to be a file specifying the pattern.
--output=[STRING]             Mandatory, has to be a directory.

```

Example:

```
 ./aruco-pattern-write --input ../specification_file.txt  --output ../Test

```

The directory `SampleInput` provides a specification file, and `SampleOutput` has the output from running this executable.

### Directory format, input aruco-pattern-write   

Within the read directory used as an argument above, you will need to create a text file called `specification_file.txt`.  The example from directory `SampleInput` is given below. 

All of these values can be changed to reflect the wishes of the user, but highly suggest not changing `arc_code`, which is the code used to generate the dictionary of aruco patterns within OpenCV.  Also, `squaresX`*`squaresY`< `1000` given that the dictionary we have used is `cv::aruco::DICT_6X6_1000`.  

	```
	squaresX 12
	squaresY 15
	squareLength 200
	markerLength 100
	margins 100
	arc_code 11
	```

### Directory format, output aruco-pattern-write  

The output of the program is:

1. one image file, in .png format, of aruco patterns laid out in a grid pattern.  The name is determined based on the dimensions of the grid.  

2. one text file called `specification_file.txt`, that records the values used to create the grid.

## Altering

Alter the Create() function to output images with different dimensions as well as grids with more, or fewer aruco patterns.

	


