#ifndef ROI_HPP
#define ROI_HPP

#include <cv.h>
#include <highgui.h>

cv::Mat detectDial(cv::Mat& img);
cv::Mat createMask(cv::Mat& img);

#endif
