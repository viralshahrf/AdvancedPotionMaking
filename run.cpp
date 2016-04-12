#include "roi.hpp"
#include "features.hpp"
#include "utilities.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    Mat src;
    if (argc < 2) {
        cout << "Not enough arguments\n";
        return -1;
    }

    src = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    if (!src.data) {
        std::cout << "Could not load input image\n";
        return -1;
    }

    Mat srcGray;
    cvtColor(src, srcGray, CV_BGR2GRAY);
    Mat maskROI = createMask(src);
    //Mat dial = detectDial(src);
    MatND hist = getHist(src, maskROI);    
    vector<KeyPoint> keypoints;
    Mat desc = extractSURF(src, keypoints);
    Mat texture = getTexture(src, maskROI, Size(80,80));

    namedWindow("Source", CV_WINDOW_AUTOSIZE);
    imshow("Source", src);

    namedWindow("Texture", CV_WINDOW_AUTOSIZE);
    imshow("Texture", texture);

    /* Display top 10 closest images */
    string dir = argv[2];
    vector<string> files;
    int error = getDir(dir, files);
    if (error != 0) {
        cout << "Could not open the directory\n";
        return -1;
    }
    
    vector<Mat> vecMat;
    getImages(files, vecMat);
    
    /*
    vector<pair<Mat, double> > matches = matchHist(hist, vecMat, string("watch"));
    vector<pair<Mat, int> > matches = matchSURF(desc, vecMat);
    */
    vector<pair<Mat, double> > matches = matchTexture(srcGray, vecMat);

    /* Get the first 10 elements */
    for (int i = 0; i < 10 && i < matches.size(); i++) {
        vecMat[i] = matches[i].first;
        cout << "Distance: " << matches[i].second << endl;
    }

    vecMat.erase(vecMat.begin() + 10, vecMat.end());

    Mat canvas = makeCanvas(vecMat, 500, 2);

    namedWindow("All Images", CV_WINDOW_AUTOSIZE);
    imshow("All Images", canvas); 
    waitKey();
    return 0;
}
