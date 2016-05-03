#include <stdio.h>
#include <algorithm>
#include <python2.7/Python.h>

#include "roi.hpp"
#include "features.hpp"
#include "utilities.hpp"

#define RESULTS 20

PyObject* testFunction(char* fileName, int colorPriority, int texturePriority, int shapePriority, char* productCategory);

extern "C" {
    PyObject* TestFunction(char* fileName, int colorPriority, int texturePriority, int shapePriority, char* productCategory)
        {
            testFunction(fileName, colorPriority, texturePriority, shapePriority, productCategory);
        }
}

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
    cv::Mat temp = cv::Mat();
    cv::Mat clustered = clusterKmeans(&src, &srcROI);
    cv::imshow("Clustered K Means", clustered);
    cv::MatND hist = getHist(&src, &srcROI);
    std::vector<std::pair<std::string, double> > colorMeasure = matchHist(&hist, &files, doSort);

    /* Calculate texture measure distance using Local Binary Patterns */
    cv::Mat sample = getMaterialSample(&src, &srcROI, cv::Size(50, 50));
    cv::Mat texture = getTexture(&sample);
    std::vector<std::pair<std::string, double> > textureMeasure = matchTexture(&texture, &files, doSort);

    /* Calculate shape measure distance using HuMoments */
    std::vector<std::pair<std::string, double> > shapeMeasure = matchShape(&outline, &files, doSort);

    std::vector<std::pair<std::string, double> > totalMeasure;

    /* Sort the results on the file name to ensure same order for all three */
    int cSize = colorMeasure.size();
    int sSize = shapeMeasure.size();
    int tSize = textureMeasure.size();
    int numResults = std::min(cSize, std::min(tSize, sSize));
 
    for (int i = 0; i < numResults; i++) {
        double totalDist;
        totalDist += colorWeight*colorMeasure[i].second;
        totalDist += textureWeight*textureMeasure[i].second;
        totalDist += shapeWeight*shapeMeasure[i].second;
        totalMeasure.push_back(std::make_pair(colorMeasure[i].first, totalDist));
    }

    std::sort(totalMeasure.begin(), totalMeasure.end(), distCompareAsc);
    numResults = std::min(numResults, RESULTS);
    std::vector<std::string> results;
    for (int i = 0; i < numResults; i++) {
        results.push_back(totalMeasure[i].first);
        printf("Dist %d: %.2f\n", i, totalMeasure[i].second);
    }

    return results;
}

PyObject* testFunction(char* fileName, int colorPriority, int texturePriority, int shapePriority, char* productCategory) {

    PyObject *result = PyList_New(0);

    printf("File: %s | Color Priority: %d | Texture Priority: %d | Shape Priority: %d | Product Category: %s\n", fileName, colorPriority, texturePriority, shapePriority, productCategory);

    int sumPriorities = colorPriority + texturePriority + shapePriority;
    double colorWeight = (double)colorPriority / (double)sumPriorities;
    double textureWeight = (double)texturePriority / (double)sumPriorities;
    double shapeWeight = (double)shapePriority / (double)sumPriorities;

    vector<string> resultImages = searchImages(fileName, colorWeight, textureWeight, shapeWeight, productCategory);

    for (int i = 0; i < resultImages.size(); i++) {
        PyList_Append(result, PyString_FromFormat(resultImages[i].c_str()));
    }

    std::cout << "Done\n";
    return result;
}
