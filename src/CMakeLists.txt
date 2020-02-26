cmake_minimum_required(VERSION 3.0)

project(camera-as-scanner-aruco)

# set the flags, may want to add/change debugging or optimization (-O3) flags as appropriate
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -std=c++11 ")

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

# OpenCV required, can specify version.  OpenCV 4.0 not strictly necessary, see the README for more details.
find_package( OpenCV REQUIRED ) 


add_executable(camera-as-scanner
camera-as-scanner.cpp
camera_calibration.cpp
camera_calibration.hpp
DirectoryFunctions.hpp
DirectoryFunctions.cpp
Includes.hpp
)

target_link_libraries(camera-as-scanner Eigen3::Eigen ${OpenCV_LIBS})

set_property(TARGET camera-as-scanner PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS camera-as-scanner DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )


# set the flags, may want to add/change debugging or optimization (-O3) flags as appropriate
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -std=c++11 ")

# OpenCV required, can specify version.  OpenCV 4.0 not strictly necessary, see the README for more details.
find_package( OpenCV REQUIRED ) 


add_executable(aruco-pattern-write
aruco-pattern-write-project.cpp
)

target_link_libraries(aruco-pattern-write ${OpenCV_LIBS})

set_property(TARGET aruco-pattern-write PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS aruco-pattern-write  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )


