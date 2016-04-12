#ifndef FEATURES_HPP
#define FEATURES_HPP

#include "roi.hpp"
#include <string>
#include <opencv2/nonfree/features2d.hpp>

cv::MatND getHist(cv::Mat& img, cv::Mat& mask, int hbins = 30, int sbins = 32);
void showHist(cv::MatND& hist, int hbins = 30, int sbins = 32);
cv::Mat extractSURF(cv::Mat& img, std::vector<cv::KeyPoint>& keypoints);
void showKeypoints(cv::Mat& img, std::vector<cv::KeyPoint>& keypoints);
cv::Mat getTexture(cv::Mat& img, cv::Mat& mask, cv::Size sampleSize);
bool distCompare(const std::pair<cv::Mat, double>& first, 
                 const std::pair<cv::Mat, double>& second);
std::vector<std::pair<cv::Mat, double> > matchHist(cv::MatND& srcHist,
                                                   std::vector<cv::Mat>& targetList, 
                                                   std::string type = std::string("necktie"));
std::vector<std::pair<cv::Mat, int> > matchSURF(cv::Mat& imgDesc,
                                                std::vector<cv::Mat>& targetList);
std::vector<std::pair<cv::Mat, double> > matchTexture(cv::Mat& imgSample, 
                                                      std::vector<cv::Mat>& targetList);
#endif
