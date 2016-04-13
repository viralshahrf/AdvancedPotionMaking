#include "roi.hpp"

using namespace cv;
using namespace std;

Mat detectDial(Mat *img) {
    Mat src;
    img->copyTo(src);
    
    /* Create Gray Image */
    Mat srcGray;
    cvtColor(src, srcGray, CV_BGR2GRAY);
    
    /* Detect Circles */
    vector<Vec3f> circles;
    int invAccRatio = 2;
    int minDist = srcGray.rows/4;
    int cannyHighThresh = 200;
    int accVotesThresh = 100;
    HoughCircles(srcGray, circles, CV_HOUGH_GRADIENT, invAccRatio, minDist, 
                 cannyHighThresh, accVotesThresh);

    if (circles.size() == 0) {
        cout << "It is not a watch or the dial isn't circular\n";
        return Mat();
    }
    
    /* Find the circle with the maximum radius */
    int maxRadius = 0, maxIndex = 0;
    for (int i = 0; i < circles.size(); i++) {
        int radius = circles[i][2];
        if (radius > maxRadius) {
            maxIndex = i;
            maxRadius = radius;
        }
    }

    /* Create a mask for the dial */
    Mat dialMask = Mat::zeros(srcGray.size(), CV_8UC1);
    Point center(cvRound(circles[maxIndex][0]), cvRound(circles[maxIndex][1])); 
    circle(dialMask, center, maxRadius, Scalar(255), CV_FILLED);

    //namedWindow("Dial Area", CV_WINDOW_AUTOSIZE);
    //imshow("Dial Area", dialMask);
    
    return dialMask;
}

Mat createMask(Mat *img, bool returnOutline, vector<Point> *outline) {
    Mat src;
    img->copyTo(src);

    /* Create Gray Image */
    Mat srcGray;
    cvtColor(src, srcGray, CV_BGR2GRAY);
    GaussianBlur(srcGray, srcGray, Size(9, 9), 2, 2);

    /* Canny edge detection */
    Mat srcEdges;
    int threshold = 50, kernel_size = 3;
    Canny(srcGray, srcEdges, threshold, threshold*2, kernel_size);
    blur(srcEdges, srcEdges, Size(3,3) );

    /* Find contours for ROI */
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(srcEdges, contours, hierarchy, CV_RETR_TREE, 
                 CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    
    /* Find the contour with maximum area */
    int maxArea = 0, maxIndex = 0;
    for (int i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i], false);
        if (area > maxArea) {
            maxArea = area;
            maxIndex = i;
        }
    }
    
    /* create a mask for the ROI */
    Mat srcMask = Mat::zeros(srcEdges.size(), CV_8UC3);
    drawContours(srcMask, contours, maxIndex, Scalar::all(255), CV_FILLED);
    if (returnOutline == true)
        *outline = contours[maxIndex];

    cvtColor(srcMask, srcMask, CV_BGR2GRAY);

    //namedWindow("Region of Interest", CV_WINDOW_AUTOSIZE);
    //imshow("Region of Interest", srcMask);
    
    return srcMask;
}

Mat getMaterialSample(Mat *img, Mat *mask, Size sampleSize) {
    Mat src;
    img->copyTo(src);

    Mat srcGray, srcEdges;
    cvtColor(src, srcGray, CV_BGR2GRAY);
    /* Canny edge detection */
    int threshold = 50, kernel_size = 3;
    Canny(srcGray, srcEdges, threshold*2, threshold*4, kernel_size);


    /* Create Gray Image */
    Mat maskGray;
    mask->copyTo(maskGray);
    if (maskGray.type() != CV_8UC1) {
        cvtColor(maskGray, maskGray, CV_BGR2GRAY);
    }
    GaussianBlur(maskGray, maskGray, Size(9, 9), 2, 2);

    /* Canny edge detection */
    Mat maskEdges;
    Canny(maskGray, maskEdges, threshold, threshold*2, kernel_size);
    blur(maskEdges, maskEdges, Size(3,3) );

    /* Find contours for ROI */
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(maskEdges, contours, hierarchy, CV_RETR_TREE, 
                 CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    
    if (contours.size() < 1) {
        std::cout << "Mask is empty\n";
        return Mat();
    }

    /* Get the moments */
    Moments mu = moments(contours[0]);

    /* Get the mass centers */
    Point2f mc = Point2f(mu.m10/mu.m00 , mu.m01/mu.m00);

    /* Get a sample of the given size */
    Mat textureSample = Mat::zeros(sampleSize, CV_8UC3);
    srcGray(Rect(mc, sampleSize)).copyTo(textureSample);

    //namedWindow("Texture Sample", CV_WINDOW_AUTOSIZE);
    //imshow("Texture Sample", textureSample);

    return textureSample;
}
