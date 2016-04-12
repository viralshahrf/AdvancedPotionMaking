#include "features.hpp"

using namespace cv;
using namespace std;

/*
** Display histogram
*/
void showHist(MatND& hist, int hbins, int sbins) {
    int scale = 10;
    Mat histImg = Mat::zeros(sbins*scale, hbins*scale, CV_8UC3);

    double maxVal = 0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);

    for (int h = 0; h < hbins; h++) {
        for (int s = 0; s < sbins; s++) {
            float binVal = hist.at<float>(h, s);
            int intensity = cvRound(binVal*255/maxVal);
            rectangle(histImg, Point(h*scale, s*scale),
                      Point( (h+1)*scale - 1, (s+1)*scale - 1),
                      Scalar::all(intensity),
                      CV_FILLED);
        }
    }
    
    namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
    imshow("Histogram", histImg);            
}

/*
** Display SURF keypoints on the image
*/
void showKeypoints(Mat& img, vector<KeyPoint>& keypoints) {
    Mat src;
    img.copyTo(src);
    drawKeypoints(src, keypoints, src);

    namedWindow("SURF Keypoints", CV_WINDOW_AUTOSIZE);
    imshow("SURF Keypoints", src);
}

/*
** Get the histogram of the area of interest
*/
MatND getHist(Mat& img, Mat& mask, int hbins, int sbins) {
    Mat src, srcMask, srcHSV;
    img.copyTo(src);
    mask.copyTo(srcMask);
    
    /* Convert the image to HSV format */
    cvtColor(src, srcHSV, CV_BGR2HSV);

    namedWindow("HSV Image", CV_WINDOW_AUTOSIZE);
    imshow("HSV Image", srcHSV);
    
    /* Calculate histogram */
    const int histSize[] = {hbins, sbins};
    float hranges[] = {0, 180};
    float sranges[] = {0, 256};
    const float* ranges[] = {hranges, sranges};
    int channels[] = {0, 1};

    MatND hist;
    int numImages = 1, numDims = 2; 
    calcHist(&srcHSV, numImages, channels, srcMask, hist, numDims, histSize, ranges);
    normalize(hist, hist);
    
    //showHist(hist, hbins, sbins);
    return hist;
}

/* 
** Compare function to rank images based on histogram match 
*/
bool distCompare(const pair<Mat, double>& elem1, const pair<Mat, double>& elem2) {
    return (elem1.second > elem2.second);
}
 
/* 
** Match histograms and rank them in the ascending order of how close they are 
** to the source 
*/
vector<pair<Mat, double> > matchHist(MatND& srcHist, vector<Mat>& targetList, string type) {
    vector<pair<Mat, double> > resultList;
    for (int i = 0; i < targetList.size(); i++) {
        Mat mask = createMask(targetList[i]);
        if (type.compare("watch") == 0) {
            mask = detectDial(targetList[i]);
        }
        MatND targetHist = getHist(targetList[i], mask);
        double dist = compareHist(srcHist, targetHist, CV_COMP_CORREL);
        resultList.push_back(make_pair(targetList[i], dist));
    }

    sort(resultList.begin(), resultList.end(), distCompare);
    return resultList;
}

/*
** Extract SURF keypoints
*/
Mat extractSURF(Mat& img, vector<KeyPoint>& keypoints) {
    Mat src;
    img.copyTo(src);

    cvtColor(src, src, CV_BGR2GRAY);
    
    /* Extract Keypoints */
    SurfFeatureDetector detector(400);
    detector.detect(src, keypoints);

    /* Compute Descriptors */
    SurfDescriptorExtractor extractor;
    Mat descriptor;
    extractor.compute(src, keypoints, descriptor);
    
    //showKeypoints(src, keypoints);
    return descriptor;    
}

vector<DMatch> filterSURFMatches(Mat& srcDesc, vector<DMatch> matches) {
    vector <DMatch> goodMatches;
    double maxDist = 0; double minDist = 100;

    /* Quick calculation of max and min distances between keypoints */
    for( int i = 0; i < srcDesc.rows; i++ ) {
        double dist = matches[i].distance;
        if( dist < minDist ) minDist = dist;
        if( dist > maxDist ) maxDist = dist;
    }

    /* 
    ** "good" matches: matches whose distance is less than 2*minDist,
    ** or a small arbitary value ( 0.02 ) in the event that minDist is very
    ** small)
    ** PS.- radiusMatch can also be used here.
    */

    for( int i = 0; i < srcDesc.rows; i++ ) {
        if (matches[i].distance <= max(2*minDist, 0.02)) {
            goodMatches.push_back(matches[i]); 
        }
    }
    
    return goodMatches;
}

vector<pair<Mat, int> > matchSURF(Mat& imgDesc, vector<Mat>& targetList) {
    vector<pair<Mat, int> > resultList;

    Mat srcDesc;
    imgDesc.copyTo(srcDesc);
    for (int i = 0; i < targetList.size(); i++) {
        /* Extract descriptors of target */
        vector<KeyPoint> keypoints;
        Mat targetDesc = extractSURF(targetList[i], keypoints);
        
        /* Identify matches with the source descriptor */
        FlannBasedMatcher matcher;
        std::vector< DMatch > matches;
        matcher.match(srcDesc, targetDesc, matches);
        /* filter Matches based on the distance */
        matches = filterSURFMatches(srcDesc, matches);
        resultList.push_back(make_pair(targetList[i], matches.size()));
    }    
    
    sort(resultList.begin(), resultList.end(), distCompare);
    return resultList;
}

vector<pair<Mat, double> > matchTexture(Mat& imgSample, vector<Mat>& targetList) {
    vector<pair<Mat, double> > resultList;
    
    Mat srcSample;
    imgSample.copyTo(srcSample);
    int h = srcSample.rows;
    int w = srcSample.cols;

    for (int i = 0; i < targetList.size(); i++) {
        Mat mask = createMask(targetList[i]);
        Mat targetTexture = getTexture(targetList[i], mask, Size(50, 50));
        int rCols =  w - targetTexture.cols + 1;
        int rRows = h - targetTexture.rows + 1;
        
        Mat result = Mat(rRows, rCols, CV_32FC1);
        matchTemplate(srcSample, targetTexture, result, CV_TM_CCORR_NORMED);
        normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
        double maxVal = 0.;
        minMaxLoc(result, 0, &maxVal, 0, 0);
        resultList.push_back(make_pair(targetList[i], maxVal));
    }

    sort(resultList.begin(), resultList.end(), distCompare);
    return resultList;
}
