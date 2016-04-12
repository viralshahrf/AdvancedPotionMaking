#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <cv.h>
#include <highgui.h>
#include <string>
#include <dirent.h>
#include <errno.h>

int getDir(std::string dir, std::vector<std::string>& files);
void getImages(std::vector<std::string>& files, std::vector<cv::Mat>& vecMat);
cv::Mat makeCanvas(std::vector<cv::Mat>& vecMat, int windowHeight, int nRows);

#endif
