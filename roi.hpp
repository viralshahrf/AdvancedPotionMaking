#ifndef ROI_HPP
#define ROI_HPP

//#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

cv::Mat detectDial(cv::Mat *img);

cv::Mat createMask(cv::Mat *img, bool returnOutline = false, std::vector<cv::Point> *outline = NULL);

cv::Mat getMaterialSample(cv::Mat *img, cv::Mat *mask, cv::Size sampleSize);

#endif
