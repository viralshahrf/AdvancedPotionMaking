#include "roi.hpp"
#include "features.hpp"
#include "utilities.hpp"

#define RESULTS 20

using namespace cv;
using namespace std;

std::vector<std::string> searchImages(char* fileName, double colorWeight, double textureWeight, double shapeWeight, char* productCategory) {
    cv::Mat src = cv::imread(fileName);
    
    /* Get the ROI from the imge */
    std::vector<cv::Point> outline;
    cv:: Mat srcROI = createMask(&src, true, &outline);
    
    /* Get the target file list */
    std::vector<std::string> files;
    int error = getDir(productCategory, files);

    bool doSort = false;

    /* Calculate color measure distance using color histograms */
    cv::MatND hist = getHist(&src, &srcROI);
    std::vector<std::pair<std::string, double> > colorMeasure = matchHist(&hist, &files, doSort);

    /* Calculate texture measure distance using Local Binary Patterns */
    cv::Mat sample = getMaterialSample(&src, &srcROI, cv::Size(50, 50));
    cv::Mat texture = getTexture(&sample);
    std::vector<std::pair<std::string, double> > textureMeasure = matchTexture(&texture, &files, doSort);

    /* Calculate shape measure distance using HuMoments */
    std::vector<std::pair<std::string, double> > shapeMeasure = matchShape(&srcROI, &files, doSort);

    std::vector<std::pair<std::string, double> > totalMeasure;
    //int numResults = std::min(std::min(RESULTS, colorMeasure.size()), std::min(textureMeasure.size(), shapeMeasure.size()));

    int numResults = RESULTS;
    for (int i = 0; i < numResults; i++) {
        double totalDist = colorWeight*colorMeasure[i].second + textureWeight*textureMeasure[i].second + shapeWeight*shapeMeasure[i].second;
        totalMeasure.push_back(std::make_pair(colorMeasure[i].first, totalDist));
    }

    std::sort(totalMeasure.begin(), totalMeasure.end(), distCompareDesc);

    std::vector<std::string> results;
    for (int i = 0; i < totalMeasure.size(); i++) {
        results.push_back(totalMeasure[i].first);
    }

    return results;
}

int main(int argc, char** argv) {
    
    vector<string> results;
    string srcName = "./Neckties/1.jpg";
    string productCategory = "Necties";
    results = searchImages("./Neckties/1.jpg", 0.33, 0.33, 0.34, "Neckties");
    
    Mat src = imread(srcName, CV_LOAD_IMAGE_COLOR);
    namedWindow("Source", CV_WINDOW_AUTOSIZE);
    imshow("Source", src);

    vector<Mat> vecMat;
    getImages(results, vecMat);
    Mat canvas = makeCanvas(vecMat, 500, 4);

    namedWindow("All Images", CV_WINDOW_AUTOSIZE);
    imshow("All Images", canvas);

    waitKey();
    return 0;

}
