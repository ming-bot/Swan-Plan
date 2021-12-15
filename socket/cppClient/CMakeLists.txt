cmake_minimum_required(VERSION 3.10)

project(socketClientCpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV REQUIRED)
find_package(realsense2 REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(include/
                    ${OpenCV_INCLUDE_DIRS}
                    ${EIGEN3_INCLUDE_DIR}
                    ${realsense2_INCLUDE_DIRS})

add_executable(cvAlgorithmsClientSocketCpp cvAlgorithmsClientCpp.cpp src/base64Converter.cpp)
target_link_libraries(cvAlgorithmsClientSocketCpp ${OpenCV_LIBRARIES} ${realsense2_LIBRARY})
