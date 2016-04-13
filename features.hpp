#ifndef FEATURES_HPP
#define FEATURES_HPP

#include "roi.hpp"
#include "lbp.hpp"

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"


using namespace std;
using namespace cv;

MatND getHist(Mat *img, Mat *mask, int bBins = 32, int gBins = 32, int rBins = 32);

void clusterKmeans(Mat *img, const Mat *mask);

Mat extractSURF(Mat *img, vector<KeyPoint> *keypoints);

Mat getTexture(Mat *img);

Mat getHist1D(Mat *img, int nBins = 32);

bool distCompareDesc(const pair<string, double> elem1, const pair<string, double> elem2);

bool distCompareAsc(const pair<string, double> elem1, const pair<string, double> elem2);

vector<pair<string, double> > matchHist(MatND *srcHist, vector<string> *targetList,
                                     bool doSort = true);

vector<pair<string, int> > matchSURF(Mat *imgDesc, vector<string> *targetList,
                                  bool doSort = true);

vector<pair<string, double> > matchTexture(Mat *imgTexture, vector<string> *targetList,
                                        bool doSort = true);

vector<pair<string, double> > matchShape(Mat *img, vector<string> *targetList,
                                      bool doSort = true);

#endif
