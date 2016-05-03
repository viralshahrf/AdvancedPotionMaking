#include "features.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

/*
** Display histogram
*/
void showHist(MatND *hist, int hbins, int sbins) {
    int scale = 10;
    Mat histImg = Mat::zeros(sbins*scale, hbins*scale, CV_8UC3);

    double maxVal = 0;
    minMaxLoc(*hist, 0, &maxVal, 0, 0);

    for (int h = 0; h < hbins; h++) {
        for (int s = 0; s < sbins; s++) {
            float binVal = hist->at<float>(h, s);
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
void showKeypoints(Mat *img, vector<KeyPoint> *keypoints) {
    Mat src;
    img->copyTo(src);
    drawKeypoints(src, *keypoints, src);

    namedWindow("SURF Keypoints", CV_WINDOW_AUTOSIZE);
    imshow("SURF Keypoints", src);
}

/*
** Show image after colour segmentation using kmeans
*/
Mat showClusters(const Mat *labels, const Mat *centers, int h, int w) {
    assert(labels->type() == CV_32SC1);
    assert(centers->type() == CV_32FC1);
 
    cv::Mat result(h, w, CV_8UC3);
    cv::MatIterator_<cv::Vec3b> resBegin = result.begin<cv::Vec3b>();
    cv::MatIterator_<cv::Vec3b> resEnd = result.end<cv::Vec3b>();
    cv::MatConstIterator_<int> labelBegin = labels->begin<int>();

    cv::Mat centersU8;
    centers->convertTo(centersU8, CV_8UC1, 255.0);
    cv::Mat centersU8C3 = centersU8.reshape(3);

    while ( resBegin != resEnd ) {
            const cv::Vec3b& rgb = centersU8C3.ptr<cv::Vec3b>(*labelBegin)[0];
            *resBegin = rgb;
            ++resBegin;
            ++labelBegin;
    }
    return result;
}

/*
** Get the BGR histogram of the area of interest
*/
MatND getHist(Mat *img, Mat *mask, int bBins, int gBins, int rBins) {
    Mat src, srcMask;
    img->copyTo(src);
    mask->copyTo(srcMask);

    /* Calculate histogram */
    const int histSize[] = {bBins, gBins, rBins};
    float bRanges[] = {0, 256};
    float gRanges[] = {0, 256};
    float rRanges[] = {0, 256};
    const float* ranges[] = {bRanges, gRanges, bRanges};
    int channels[] = {0, 1, 2};

    MatND hist;
    int numImages = 1, numDims = 3;
    calcHist(&src, numImages, channels, srcMask, hist, numDims, histSize, ranges);
    normalize(hist, hist);
    
    return hist;
}

/*
** Find dominant colours using K-Means segmentation
*/
Mat clusterKmeans(Mat *img, const Mat *mask) {
    Mat src, srcMask;
    mask->copyTo(srcMask);
    img->copyTo(src, srcMask);
    //imshow("original", src);
    //std::cout << "src: " << src.rows << ", " << src.cols << std::endl;
    assert(src.type() == CV_8UC3);

    cv::Mat srcFlat = src.reshape(1, src.cols * src.rows);
    assert(srcFlat.type() == CV_8UC1);

    cv::Mat srcFlat32f;
    srcFlat.convertTo(srcFlat32f, CV_32FC1, 1.0 / 255.0);
    assert(srcFlat32f.type() == CV_32FC1);

    cv::Mat labels;
    int nClusters = 5, nAttempts = 5;
    cv::TermCriteria criteria {cv::TermCriteria::COUNT, 100, 1};
    cv::Mat centers;
    cv::kmeans(srcFlat32f, nClusters, labels, criteria, nAttempts,
               cv::KMEANS_RANDOM_CENTERS, centers);

    return showClusters(&labels, &centers, src.rows, src.cols);
}

/*
** Extract SURF keypoints
*/
Mat extractSURF(Mat *img, vector<KeyPoint> *keypoints) {
    Mat src;
    img->copyTo(src);

    //initModule_nonfree();
    cvtColor(src, src, CV_BGR2GRAY);
    
    // Extract Keypoints
    Ptr<SURF> surfFeature = SURF::create(400);
    surfFeature->detect(src, *keypoints);

    // Compute Descriptors
    //xfeatures2d::SurfDescriptorExtractor extractor;
    Mat descriptor;
    surfFeature->compute(src, *keypoints, descriptor);
    
    showKeypoints(&src, keypoints);
    return descriptor;
}

/*
** Filter good SURF matches
** "good" matches: matches whose distance is less than 2*minDist,
** or a small arbitary value ( 0.02 ) in the event that minDist is very
** small)
** PS.- radiusMatch can also be used here.
*/
vector<DMatch> filterSURFMatches(Mat *srcDesc, vector<DMatch> *matches) {
    vector <DMatch> goodMatches;
    double maxDist = 0; double minDist = 100;

    /* Quick calculation of max and min distances between keypoints */
    for( int i = 0; i < srcDesc->rows; i++ ) {
        double dist = (*matches)[i].distance;
        if( dist < minDist ) minDist = dist;
        if( dist > maxDist ) maxDist = dist;
    }

    for( int i = 0; i < srcDesc->rows; i++ ) {
        if ((*matches)[i].distance <= max(2*minDist, 0.02)) {
            goodMatches.push_back((*matches)[i]);
        }
    }
    
    return goodMatches;
}

/*
** Get the texture using Local Binary Pattern feature
*/
Mat getTexture(Mat *img) {
    Mat src, lbp;
    img->copyTo(src);
    int radius = 1;
    int neighbours = 8;

    lbp::VARLBP(src, lbp, radius, neighbours);
    normalize(lbp, lbp, 0, 255, NORM_MINMAX, CV_8UC1);

    //imshow("LBP", lbp);

    return lbp;
}

/*
** Compute 1D histogram of graysclae image
** Called to compute histogram of the LBP texture feature
*/
Mat getHist1D(Mat *img, int nBins) {
    Mat src;
    img->copyTo(src);

    const int histSize[] = {nBins};
    float nRanges[] = {0, 256};
    const float* ranges[] = {nRanges};
    const int channels[] = {0};

    MatND hist;
    int numImages = 1, numDims = 1;
    calcHist(&src, numImages, channels, Mat(), hist, numDims, histSize, ranges);
    //normalize(hist, hist);

    return hist;
}

/*
** Compare function to rank images based on histogram match in descending order of
** the distance parameter
*/
bool distCompareDesc(const pair<string, double> elem1, const pair<string, double> elem2) {
    return (elem1.second > elem2.second);
}

/*
** Compare function to rank images based on histogram match in descending order of
** the distance parameter
*/
bool distCompareAsc(const pair<string, double> elem1, const pair<string, double> elem2) {
    return (elem1.second < elem2.second);
}

/*
** Match histograms and rank them in the ascending order of how close they are
** to the source
*/
vector<pair<string, double> > matchHist(MatND *srcHist, vector<string> *targetList, bool doSort) {
    vector<pair<string, double> > resultList;
    for (int i = 0; i < targetList->size(); i++) {
        Mat target = imread((*targetList)[i], CV_LOAD_IMAGE_COLOR);
        Mat mask = createMask(&target);
        MatND targetHist = getHist(&target, &mask);
        double dist = compareHist(*srcHist, targetHist, CV_COMP_CHISQR);
        resultList.push_back(make_pair((*targetList)[i], dist));
    }

    if (doSort == true)
        sort(resultList.begin(), resultList.end(), distCompareAsc);

    return resultList;
}

/*
** Match based on SURF feature points
*/
vector<pair<string, double> > matchSURF(Mat *imgDesc, vector<string> *targetList, bool doSort) {
    vector<pair<string, double> > resultList;

    Mat srcDesc;
    imgDesc->copyTo(srcDesc);
    for (int i = 0; i < targetList->size(); i++) {
        //Extract descriptors of target
        Mat target = imread((*targetList)[i], CV_LOAD_IMAGE_COLOR);
        vector<KeyPoint> keypoints;
        Mat targetDesc = extractSURF(&target, &keypoints);
        
        //Identify matches with the source descriptor
        FlannBasedMatcher matcher;
        std::vector< DMatch > matches;
        matcher.match(srcDesc, targetDesc, matches);

        //filter Matches based on the distance
        matches = filterSURFMatches(&srcDesc, &matches);
        resultList.push_back(make_pair((*targetList)[i], -(double)matches.size()));
    }    
    
    if (doSort == true)
        sort(resultList.begin(), resultList.end(), distCompareAsc);

    return resultList;
}

/*
** Match LBP texture pattern of samples taken from the image using histograms
*/
vector<pair<string, double> > matchTexture(Mat *imgTexture, vector<string> *targetList, bool doSort) {
    vector<pair<string, double> > resultList;
    
    Mat srcTexture;
    imgTexture->copyTo(srcTexture);
    int h = srcTexture.rows;
    int w = srcTexture.cols;
    Mat srcHist = getHist1D(&srcTexture, 32);

    for (int i = 0; i < targetList->size(); i++) {
        Mat target = imread((*targetList)[i], CV_LOAD_IMAGE_COLOR);
        Mat mask = createMask(&target);
        Mat targetSample = getMaterialSample(&target, &mask, Size(50, 50));
        Mat targetTexture = getTexture(&targetSample);
        int rCols =  w - targetTexture.cols + 1;
        int rRows = h - targetTexture.rows + 1;
        
        Mat result = Mat(rRows, rCols, CV_32FC1);

        Mat targetHist = getHist1D(&targetTexture, 32);

        double dist = compareHist(srcHist, targetHist, CV_COMP_CHISQR);

        resultList.push_back(make_pair((*targetList)[i], dist));
    }

    if (doSort == true)
        sort(resultList.begin(), resultList.end(), distCompareAsc);

    return resultList;
}

/*
** Match products based on the contours defining their outline
*/
vector<pair<string, double> > matchShape(Mat *img, vector<string> *targetList, bool doSort) {
    vector<pair<string, double> > resultList;

    Mat src;
    img->copyTo(src);
    vector<Point> srcShape;
    Mat srcMask = createMask(&src, true, &srcShape);

    for (int i = 0; i < targetList->size(); i++) {
        Mat target = imread((*targetList)[i], CV_LOAD_IMAGE_COLOR);
        if (!target.data)
            continue;
        vector<Point> targetShape;
        Mat mask = createMask(&target, true, &targetShape);

        double dist = matchShapes(srcShape, targetShape, CV_CONTOURS_MATCH_I1, 0.0);
        resultList.push_back(make_pair((*targetList)[i], dist));
    }

    if (doSort == true)
        sort(resultList.begin(), resultList.end(), distCompareDesc);

    return resultList;
}

/*
** Match products based on the contours defining their outline
*/
vector<pair<string, double> > matchShape(vector<Point> *srcOutline, vector<string> *targetList, bool doSort) {
    vector<pair<string, double> > resultList;
    Ptr<ShapeContextDistanceExtractor> srcShapeContext = cv::createShapeContextDistanceExtractor();

    for (int i = 0; i < targetList->size(); i++) {
        Mat target = imread((*targetList)[i], CV_LOAD_IMAGE_COLOR);
        if (!target.data)
            continue;
        vector<Point> targetOutline;
        Mat tempMask = createMask(&target, true, &targetOutline);
        double dist = srcShapeContext->computeDistance(*srcOutline, targetOutline);
        resultList.push_back(make_pair((*targetList)[i], dist));
    }

    if (doSort == true)
        sort(resultList.begin(), resultList.end(), distCompareAsc);

    return resultList;
}
