#ifndef FEATURES_HPP
#define FEATURES_HPP

#include "roi.hpp"
#include "lbp.hpp"

#include <opencv2/opencv_modules.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
//#include <opencv2/nonfree/features2d.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/shape.hpp>
#include <iostream>



using namespace std;
using namespace cv;

MatND getHist(Mat *img, Mat *mask, int bBins = 6, int gBins = 6, int rBins = 6);

Mat clusterKmeans(Mat *img, const Mat *mask);

Mat extractSURF(Mat *img, vector<KeyPoint> *keypoints);

Mat getTexture(Mat *img);

Mat getHist1D(Mat *img, int nBins = 32);

bool distCompareDesc(const pair<string, double> elem1, const pair<string, double> elem2);

bool distCompareAsc(const pair<string, double> elem1, const pair<string, double> elem2);

bool nameCompareAsc(const pair<string, double> elem1, const pair<string, double> elem2);

vector<pair<string, double> > matchHist(MatND *srcHist, vector<string> *targetList,
                                     bool doSort = true);

double matchHist(string *srcFile, string *targetFile);

vector<pair<string, double> > matchSURF(Mat *imgDesc, vector<string> *targetList,
                                  bool doSort = true);

double matchSURF(string *srcFile, string *targetFile);

vector<pair<string, double> > matchTexture(Mat *imgTexture, vector<string> *targetList,
                                        bool doSort = true);

double matchTexture(string *srcFile, string *targetFile);

vector<pair<string, double> > matchShape(Mat *img, vector<string> *targetList,
                                      bool doSort = true);

double matchShape(string *srcFile, string *targetFile);

vector<pair<string, double> > matchShape(vector<Point> *imgOutline, vector<string> *targetList,
                                      bool doSort = true);

#endif
