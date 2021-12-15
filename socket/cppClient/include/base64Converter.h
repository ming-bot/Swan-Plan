/*
copyright: cited from https://github.com/RonnyldoSilva/OpenCV_Mat_to_Base64/blob/master/ConvertImage.h
*/
#ifndef _BASE64_CONVERTER_H
#define _BASE64_CONVERTER_H

// include the std cpp header files
#include <iostream>
#include <vector>
#include <string>

// include the opencv header files
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

namespace base64 {
std::string base64_encode(uchar const* bytesToEncode, unsigned int nLength);
std::string base64_decode(std::string const& encodedString);
}

#endif