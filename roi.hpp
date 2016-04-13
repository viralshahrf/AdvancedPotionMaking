#ifndef ROI_HPP
#define ROI_HPP

#include <cv.h>
#include <highgui.h>

cv::Mat detectDial(cv::Mat *img);

cv::Mat createMask(cv::Mat *img, bool returnOutline = false, std::vector<cv::Point> *outline = NULL);

cv::Mat getMaterialSample(cv::Mat *img, cv::Mat *mask, cv::Size sampleSize);

#endif
